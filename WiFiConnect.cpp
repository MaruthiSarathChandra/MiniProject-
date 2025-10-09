
#include <WiFi.h>
#include "WiFiConnect.h"
#include "Application.h"   // contains WIFI_SSID and WIFI_PASS




ApplicationConfig appConfig;  // Create global config object


// Function: Connect to Wi-Fi network
void WiFiConnect::connectToWiFi() {
  Serial.printf("\n[WiFi] Connecting to network... SSID: %s\n", appConfig.getSSID());

  WiFi.mode(WIFI_STA);
  WiFi.begin(appConfig.getSSID(), appConfig.getPassword());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connected successfully!");
    Serial.print("[WiFi] IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WiFi] Name: ");
    Serial.println(appConfig.getSSID());
  } else {
    Serial.println("\n❌ WiFi Connection Failed! Re-check SSID and password.");
    WiFi.disconnect(true);
    delay(1000);
  }
}







// Function: Check current connection status
bool WiFiConnect::isWiFiConnected() {
  return (WiFi.status() == WL_CONNECTED);
}



// Function: Automatically reconnect if Wi-Fi disconnects
void WiFiConnect::ensureWiFiConnection() {
  if (!isWiFiConnected()) {
    WiFi.disconnect(true);
    Serial.println("[WiFi] Connection lost! Attempting to reconnect...");
    connectToWiFi();
  }
}
