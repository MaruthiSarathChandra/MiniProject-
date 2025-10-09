#include "RestApiServer.h"
#include <WiFi.h>
#include <HTTPClient.h>

RestApiServer::RestApiServer(DHTReader* dhtReader, HTRepo* htRepo)
    : server(80), dhtReader(dhtReader), htRepo(htRepo) {}

void RestApiServer::begin() {
    server.on("/health", HTTP_GET, [this]() { handleHealth(); });
    server.on("/sensor", HTTP_GET, [this]() { handleSensor(); });
    server.on("/config", HTTP_POST, [this]() { handleConfig(); });
    server.on("/push-now", HTTP_POST, [this]() { handlePushNow(); });

    server.begin();
}

void RestApiServer::handleClient() {
    server.handleClient();
}

void RestApiServer::handleHealth() {
    StaticJsonDocument<128> doc;
    doc["ok"] = true;
    doc["uptime_s"] = millis() / 1000;
    sendJsonResponse(doc);
}

void RestApiServer::handleSensor() {
    float h = dhtReader->readHumidity();
    float t = dhtReader->readTemperature();

    StaticJsonDocument<256> doc;
    doc["temperature"] = t;
    doc["humidity"] = h;
    doc["timestamp"] = millis() / 1000;
    sendJsonResponse(doc, encryptionEnabled);
}

void RestApiServer::handleConfig() {
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, server.arg("plain"));
    if (err) {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    if (doc.containsKey("uploadInterval"))
        uploadInterval = doc["uploadInterval"];
    if (doc.containsKey("encryption"))
        encryptionEnabled = doc["encryption"];

    StaticJsonDocument<128> response;
    response["uploadInterval"] = uploadInterval;
    response["encryption"] = encryptionEnabled;
    sendJsonResponse(response);
}

void RestApiServer::handlePushNow() {
    float h = dhtReader->readHumidity();
    float t = dhtReader->readTemperature();

    htRepo->sendSensorData(t, h);

    StaticJsonDocument<128> doc;
    doc["pushed"] = true;
    doc["temperature"] = t;
    doc["humidity"] = h;
    sendJsonResponse(doc);
}

void RestApiServer::sendJsonResponse(JsonDocument& doc, bool encrypt) {
    String payload;
    serializeJson(doc, payload);
    if (encrypt)
        payload = EDCryptorService::encryptJson(payload);

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", payload);
}
