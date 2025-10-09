#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <ArduinoJson.h>



class JsonHandler {
  public:
    // Builds JSON from sensor values and returns as a String
    static String createJson(float temperature, float humidity, const char* teamNumber);
};




#endif
