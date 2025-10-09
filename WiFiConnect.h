#ifndef WIFICONNECT_H
#define WIFICONNECT_H

#include <WiFi.h>
#include "Application.h"

class WiFiConnect {
  public:
    void connectToWiFi();
    bool isWiFiConnected();
    void ensureWiFiConnection();
};

#endif
