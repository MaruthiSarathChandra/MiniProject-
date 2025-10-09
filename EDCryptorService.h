#ifndef EDCRYPTORSERVICE_H
#define EDCRYPTORSERVICE_H

#include <Arduino.h>

class EDCryptorService {
public:
    // Encrypt a plaintext JSON string and return a JSON string containing base64 iv and ciphertext
    static String encryptJson(const String& json);

    // Decrypt the JSON produced by encryptJson and return the original plaintext JSON string
    static String decryptJson(const String& encryptedJson);

private:
    // AES-128-CBC settings
    static const size_t KEY_SIZE = 16;   // 16 bytes = 128 bits
    static const size_t IV_SIZE  = 16;   // 16 bytes for CBC IV
    // Note: TAG is not used for CBC

    // 16-byte AES key (example). Replace with your own securely.
    static const uint8_t aesKey[KEY_SIZE];

    // Base64 helpers
    static String base64Encode(const uint8_t* data, size_t len);
    static bool base64Decode(const String& input, uint8_t* output, size_t& outputLen);

    // PKCS7 padding helpers
    static uint8_t* pkcs7_pad(const uint8_t* in, size_t inLen, size_t blockSize, size_t& outLen);
    static bool pkcs7_unpad(uint8_t* buf, size_t& len, size_t blockSize);
};

#endif // EDCRYPTORSERVICE_H
