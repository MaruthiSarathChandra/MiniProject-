#include "EDCryptorService.h"
#include <mbedtls/aes.h>
#include <mbedtls/base64.h>
#include <ArduinoJson.h>
#include <esp_system.h>

// ----- AES key (16 bytes) -----
// Replace with your secure key. Keep same bytes in server-side decryption.
const uint8_t EDCryptorService::aesKey[EDCryptorService::KEY_SIZE] = {
    0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B, 0x0C,0x0D,0x0E,0x0F
};

// ------- Base64 encode helper -------
String EDCryptorService::base64Encode(const uint8_t* data, size_t len) {
    size_t outLen = 0;
    // first call to get required length
    mbedtls_base64_encode(nullptr, 0, &outLen, data, len);
    uint8_t* out = new uint8_t[outLen + 1];
    if (!out) return String();
    if (mbedtls_base64_encode(out, outLen, &outLen, data, len) != 0) {
        delete[] out;
        return String();
    }
    out[outLen] = '\0';
    String result = String((char*)out);
    delete[] out;
    return result;
}

// ------- Base64 decode helper -------
// returns true on success and sets outputLen
bool EDCryptorService::base64Decode(const String& input, uint8_t* output, size_t& outputLen) {
    // first determine required buffer length
    if (mbedtls_base64_decode(nullptr, 0, &outputLen, (const uint8_t*)input.c_str(), input.length()) != MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
        // If this call doesn't return BUFFER_TOO_SMALL, it's okay: it might return success if 0-length input.
        // But we still attempt actual decode below.
    }
    int ret = mbedtls_base64_decode(output, outputLen, &outputLen, (const uint8_t*)input.c_str(), input.length());
    return (ret == 0);
}

// ------- PKCS7 padding -------
// Pads input to multiple of blockSize; returns newly allocated buffer (caller must delete[]).
uint8_t* EDCryptorService::pkcs7_pad(const uint8_t* in, size_t inLen, size_t blockSize, size_t& outLen) {
    size_t pad = blockSize - (inLen % blockSize);
    outLen = inLen + pad;
    uint8_t* out = new uint8_t[outLen];
    if (!out) return nullptr;
    memcpy(out, in, inLen);
    // fill padding bytes with value = pad
    for (size_t i = 0; i < pad; ++i) out[inLen + i] = (uint8_t)pad;
    return out;
}

// Remove PKCS7 padding in-place; returns true on success and updates len to new length
bool EDCryptorService::pkcs7_unpad(uint8_t* buf, size_t& len, size_t blockSize) {
    if (len == 0 || (len % blockSize) != 0) return false;
    uint8_t pad = buf[len - 1];
    if (pad == 0 || pad > blockSize) return false;
    // verify padding bytes
    for (size_t i = 0; i < pad; ++i) {
        if (buf[len - 1 - i] != pad) return false;
    }
    len -= pad;
    return true;
}

// ------- encryptJson implementation (AES-128-CBC + base64) -------
String EDCryptorService::encryptJson(const String& json) {
    // generate random IV
    uint8_t iv[IV_SIZE];
    esp_fill_random(iv, IV_SIZE);

    // prepare plaintext with PKCS7 padding
    const uint8_t* input = (const uint8_t*)json.c_str();
    size_t inLen = json.length();
    size_t paddedLen = 0;
    uint8_t* padded = pkcs7_pad(input, inLen, 16, paddedLen); // AES block size = 16
    if (!padded) return String("ENCRYPTION_FAILED");

    // allocate ciphertext buffer
    uint8_t* ciphertext = new uint8_t[paddedLen];
    if (!ciphertext) {
        delete[] padded;
        return String("ENCRYPTION_FAILED");
    }

    // AES-CBC encryption
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    if (mbedtls_aes_setkey_enc(&aes, aesKey, KEY_SIZE * 8) != 0) {
        mbedtls_aes_free(&aes);
        delete[] padded;
        delete[] ciphertext;
        return String("ENCRYPTION_FAILED");
    }

    // mbedtls_aes_crypt_cbc expects an IV buffer that will be updated; copy iv into iv_copy
    uint8_t iv_copy[IV_SIZE];
    memcpy(iv_copy, iv, IV_SIZE);

    if (mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLen, iv_copy, padded, ciphertext) != 0) {
        mbedtls_aes_free(&aes);
        delete[] padded;
        delete[] ciphertext;
        return String("ENCRYPTION_FAILED");
    }

    mbedtls_aes_free(&aes);
    delete[] padded;

    // base64 encode iv and ciphertext
    String iv_b64 = base64Encode(iv, IV_SIZE);
    String ct_b64 = base64Encode(ciphertext, paddedLen);

    delete[] ciphertext;

    // build JSON { iv: "...", ciphertext: "..." }
    StaticJsonDocument<512> doc;
    doc["iv"] = iv_b64;
    doc["ciphertext"] = ct_b64;

    String out;
    serializeJson(doc, out);
    return out;
}

// ------- decryptJson implementation (AES-128-CBC + base64) -------
String EDCryptorService::decryptJson(const String& encryptedJson) {
    StaticJsonDocument<1024> doc;
    DeserializationError err = deserializeJson(doc, encryptedJson);
    if (err) return String("JSON_PARSE_ERROR");

    String iv_b64 = doc["iv"].as<String>();
    String ct_b64 = doc["ciphertext"].as<String>();

    // decode base64
    size_t ivBufLen = IV_SIZE;
    uint8_t iv[IV_SIZE];
    if (!base64Decode(iv_b64, iv, ivBufLen) || ivBufLen != IV_SIZE) {
        return String("DECODE_IV_FAILED");
    }

    // determine ciphertext length
    size_t ctLenEstimate = 0;
    // get required length by calling decode with null? mbedtls_base64_decode requires buffer; we will call first to get length:
    // mbedtls_base64_decode(nullptr, 0, &ctLenEstimate, (const uint8_t*)ct_b64.c_str(), ct_b64.length());
    // But earlier helper requires provided buffer; to be safe, compute output length roughly: (input_len * 3)/4
    ctLenEstimate = (ct_b64.length() * 3) / 4 + 4;
    uint8_t* ciphertext = new uint8_t[ctLenEstimate];
    size_t ctLen = ctLenEstimate;
    if (!base64Decode(ct_b64, ciphertext, ctLen)) {
        delete[] ciphertext;
        return String("DECODE_CT_FAILED");
    }

    // AES-CBC decrypt
    uint8_t* decrypted = new uint8_t[ctLen];
    if (!decrypted) {
        delete[] ciphertext;
        return String("DECRYPTION_FAILED");
    }

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    if (mbedtls_aes_setkey_dec(&aes, aesKey, KEY_SIZE * 8) != 0) {
        mbedtls_aes_free(&aes);
        delete[] ciphertext;
        delete[] decrypted;
        return String("DECRYPTION_FAILED");
    }

    // mbedtls_aes_crypt_cbc updates IV, so copy
    uint8_t iv_copy[IV_SIZE];
    memcpy(iv_copy, iv, IV_SIZE);

    if (mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, ctLen, iv_copy, ciphertext, decrypted) != 0) {
        mbedtls_aes_free(&aes);
        delete[] ciphertext;
        delete[] decrypted;
        return String("DECRYPTION_FAILED");
    }

    mbedtls_aes_free(&aes);
    delete[] ciphertext;

    // remove PKCS7 padding
    size_t decLen = ctLen;
    bool ok = pkcs7_unpad(decrypted, decLen, 16);
    if (!ok) {
        delete[] decrypted;
        return String("UNPAD_FAILED");
    }

    // build result string
    String result = String((char*)decrypted).substring(0, decLen);
    delete[] decrypted;
    return result;
}
