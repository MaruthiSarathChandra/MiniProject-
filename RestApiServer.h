#ifndef REST_API_SERVER_H
#define REST_API_SERVER_H

#include <WebServer.h>
#include <ArduinoJson.h>
#include "DHTReader.h"
#include "HTRepo.h"
#include "EDCryptorService.h"

class RestApiServer {
public:
    RestApiServer(DHTReader* dhtReader, HTRepo* htRepo);
    void begin();
    void handleClient();

private:
    WebServer server;
    DHTReader* dhtReader;
    HTRepo* htRepo;

    bool encryptionEnabled = true;
    unsigned long uploadInterval = 30;

    void handleHealth();
    void handleSensor();
    void handleConfig();
    void handlePushNow();
    void sendJsonResponse(JsonDocument& doc, bool encrypt = false);
};

#endif
