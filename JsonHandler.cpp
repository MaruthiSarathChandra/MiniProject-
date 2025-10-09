#include "JsonHandler.h"

String JsonHandler::createJson(float temperature, float humidity, const char* teamNumber) {
  DynamicJsonDocument doc(256);

  doc["team_number"] = teamNumber;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["timestamp"] = millis();

  String jsonString;
  serializeJson(doc, jsonString);

  return jsonString;
}
