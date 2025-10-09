#ifndef APPLICATION_H
#define APPLICATION_H

#include <Arduino.h>

class ApplicationConfig {
private:
    // Wi-Fi credentials (private for encapsulation)
    const char* wifiSSID  = "SpectrumSetup-31"; //Blackout, SpectrumSetup-31
    const char* wifiPASS  = "farmernorth746"; //9a63a39a7, farmernorth746
    const char* serverURL = "http://192.168.1.154:8888/post-data";  // Flask endpoint

public:
    // Getters
    const char* getSSID() const {
        return wifiSSID;
    }

    const char* getPassword() const {
        return wifiPASS;
    }

    const char* getServerURL() const {
        return serverURL;
    }
};

#endif  // APPLICATION_H
