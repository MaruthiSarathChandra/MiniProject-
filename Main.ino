
#include "DHTReader.h"
#include "SerialHandler.h"
#include "LightController.h"
#include <Arduino.h>


SerialHandler serialHandler;
DHTReader dhtReader;

void setup() {

  //Set up for light
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  dhtReader.begin();
  Serial.println("DHT11 test starting...");
  serialHandler.showMenu();
}

void loop() {
  serialHandler.handleInput();

  if(serialHandler.getMonitoringActive()) {

    float h = dhtReader.readHumidity();
    float t = dhtReader.readTemperature();

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
      //turnOn();
      normalBlink(50);
    } else if (t < 15 || h < 30) {
      normalBlink(200);
    } else {
      //normalBlink(200);
      digitalWrite(LED_PIN, LOW);
    }
  }
}


