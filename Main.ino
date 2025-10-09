
#include "DHTReader.h"
#include "SerialHandler.h"
#include "LightController.h"
#include "WiFiConnect.h"
#include "Application.h"
#include "HTRepo.h"
#include <Arduino.h>
#include "RestApiServer.h"


// Creating file objects
WiFiConnect wifiConnect;
SerialHandler serialHandler;
DHTReader dhtReader;
HTRepo htRepo;
RestApiServer restServer(&dhtReader, &htRepo);


void setup() {


  //Set up for light
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);              //Defining the Serial Reading Value 
  dhtReader.begin();                //dhtReader Instalizing

  wifiConnect.connectToWiFi();     // Connect using Application.h credentials

  restServer.begin();

}


void loop() {
  serialHandler.handleInput();
  restServer.handleClient();


  float h;
  float t;

  if(serialHandler.getMonitoringActive()) {

    if (wifiConnect.isWiFiConnected() == 0) {
      wifiConnect.ensureWiFiConnection();  // Auto reconnect if dropped
      delay(10000);            // Check every 10 seconds
    }

    h = dhtReader.readHumidity();
    t = dhtReader.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("FAILED HUMIDITY AND TEMPERATURE.............");
      delay(2000);
      return;
    }

    Serial.print("Humidity : ");
    Serial.print(h);
    Serial.print(" %  |  ");
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.println(" °C");

    delay(2000);





    // temp > 30 and humidity > 70 led should turn on
    // temp < 15 and humidity < 30 led blink every second
    // temp in range(15, 30) and humidity in range(30, 70) led should off.

    if (t > serialHandler.getTemperatureThreshold() || h > serialHandler.getHumidityThreshold()) {
      turnOn();
    } else if (t < serialHandler.getMinTemperatureThreshold() || h < serialHandler.getMinHumidityThreshold()) {
      normalBlink(200);
    } else {
      digitalWrite(LED_PIN, LOW);
    }


    
    htRepo.sendSensorData(t, h); // uploading data to server

  }

}


