

#include "HTRepo.h"
#include "Application.h"


extern WiFiConnect wifiConnect;
extern ApplicationConfig appConfig;  // Create global config object


//sendSensorData is a method to send data to server
void HTRepo::sendSensorData(float temperature, float humidity) {


  if (wifiConnect.isWiFiConnected()) {

    // Create JSON payload
    String jsonPayload = JsonHandler::createJson(temperature, humidity, "Team04");

    // Encrypt JSON payload
    String encryptedPayload = EDCryptorService::encryptJson(jsonPayload);

    Serial.print(encryptedPayload);
    
    
    
    //Send to Flask REST API
    HTTPClient http;
    http.begin(appConfig.getServerURL());  // From Application.h
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(encryptedPayload);

    //int httpResponseCode = 1;


    // Response handling
    if (httpResponseCode > 0) {
      Serial.printf("[HTRepo] POST Response Code: %d\n", httpResponseCode);
      String response = http.getString();
      Serial.println("[HTRepo] Server Response: " + response);
    } else {
      Serial.printf("[HTRepo] POST failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    // Ensure Wi-Fi connection
    wifiConnect.ensureWiFiConnection();
  }
}













