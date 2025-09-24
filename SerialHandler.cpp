
#include "SerialHandler.h"
#include "DHTReader.h"



extern DHTReader dhtReader;
// insliging the SerialHandler Constructor
SerialHandler::SerialHandler() {
  monitoringActive = false;

  // default thershold value of temp and humidity 
  tempThreshold = 30.0;    
  humidityThreshold = 70.0;

}

// insliging the showMenu Method
void SerialHandler::showMenu() {
  Serial.println("\n--- Serial Menu ---");
  Serial.println("Press 1 --> Start Monitoring");
  Serial.println("Press 2 --> Stop Monitoring");
  Serial.println("Press 3 --> Get Temperature and Humidity");
  Serial.println("Press 4 --> Set Temperature Threshold");
  Serial.println("Press 5 --> Set Humidity Threshold");
  Serial.println("Press 6 --> Get Temperature And Humidity Thershold");
  Serial.println("-------------------");
  Serial.print("Enter choice: ");
}


//Implementaion of the HandleInput 
void SerialHandler::handleInput() {

  // stores user input
  char input;

  if (Serial.available()) {
    // read one character
    input = Serial.read();
    Serial.readStringUntil('\n');

    switch (input) {
      case '1':
        monitoringActive = true;
        Serial.println("Monitoring Started.....");
        break;

      case '2':
        if (monitoringActive != false) {
          monitoringActive = false;
          digitalWrite(2, LOW);
          Serial.println("Monitoring Stopped.....");
        } else {
          Serial.println("DHT11 is not yet started to stop......");
        }
        break;

      case '3':
        if (monitoringActive != true) {
          Serial.print("Please Start DHT11.....");
        } else {
          Serial.print("Getting Temp:" + String(dhtReader.readTemperature()));
          Serial.print("Getting Humidity:" + String(dhtReader.readHumidity()));
        }
        break;

      case '4':
        setTemperatureThreshold();
        break;

      case '5':
        setHumidityThreshold();
        break;

      case '6':
        Serial.print("Humidity Threshold: ");
        Serial.print(getHumidityThreshold());
        Serial.print(" %  |  ");
        Serial.print("Temperature Threshold: ");
        Serial.print(getTemperatureThreshold());
        Serial.println(" °C");
        break;

      default:
        Serial.println("Invalid choice");
        break;
    }
  }

}



//setters and getters
float SerialHandler::getTemperatureThreshold() {
  return tempThreshold;
}

float SerialHandler::getHumidityThreshold() {
  return humidityThreshold;
}


// method used to set the humidity threshold
void SerialHandler::setHumidityThreshold() {
  Serial.print("Enter Humidity Value: ");

  while (!Serial.available()); // wait

  Serial.println("Updating Humidity Threshold......");
  
  humidityThreshold = Serial.parseFloat();

  delay(200);
  
  Serial.println("Updated Temperature Thershold:" + String(humidityThreshold));
}


// method used to set the temperature threshold
void SerialHandler::setTemperatureThreshold() {
  
  Serial.print("Enter Temperature Value: ");
  
  while (!Serial.available()); // wait
  
  Serial.println("Updating Temperature Threshold......");
  
  tempThreshold = Serial.parseFloat();
  
  delay(200);
  
  Serial.println("Updated Temperature Thershold:" + String(tempThreshold));

}

//getter
bool SerialHandler::getMonitoringActive() {
  return monitoringActive;
}


