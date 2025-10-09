#ifndef HTREPO_H
#define HTREPO_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "JsonHandler.h"
#include "EDcryptorService.h"
#include "Application.h"
#include "WiFiConnect.h"

class HTRepo {
  public:
    // Send encrypted sensor data to Flask server
    static void sendSensorData(float temperature, float humidity);

    // Optionally fetch configuration (thresholds) from server
    static void fetchServerConfig();
};

#endif
