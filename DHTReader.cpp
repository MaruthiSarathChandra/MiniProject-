#include "DHTReader.h"
#include "SerialHandler.h"
#include <utility>
#include <cmath>


//Constructor
DHTReader::DHTReader(): dht(DHTPIN, DHTTYPE) {
  temperatureOffSet = 0.0;
  humidityOffSet = 0.0;
  isCalibrated = false;
}





// initialize Sensor
void DHTReader::begin() {
  dht.begin();
}

// raw getters
float DHTReader::readRawHumidity() {
  Serial.print("here");
  return dht.readHumidity();
}
float DHTReader::readRawTemperature() {
  return dht.readTemperature();
}








// calibration with rolling mean
void DHTReader::calibrate() {

    //Serial.print("Entered");

    float samplesCount = 30;
    float tSum = 0.0, hSum = 0.0;

    while(samples.size() < samplesCount) {
      float t = readRawTemperature();
      float h = readRawHumidity();

      // push new sample
      samples.push({t, h});

      tSum += t;
      hSum += h;
      delay(200);
    }

    Serial.print(samples.size());
    
    //temperatureOffSet = hSum;
    //humidityOffSet = tSum;

    // only start calibration if we have enough samples
    if (samples.size() == samplesCount) {

      Serial.print("Entered");
      float newTempOffset = tSum / samplesCount;
      float newHumOffset  = hSum / samplesCount;

        // dynamic threshold
      float tempDiff = fabs(newTempOffset - temperatureOffSet);
      float humDiff  = fabs(newHumOffset - humidityOffSet);

      float tempThreshold = 0.05 * fabs(newTempOffset); // 5%
      float humThreshold  = 0.05 * fabs(newHumOffset);

      if (tempDiff > tempThreshold || humDiff > humThreshold) {
        temperatureOffSet = newTempOffset;
        humidityOffSet = newHumOffset;
      }

      // stabilization check
      if (tempDiff < 0.5 && humDiff < 0.5) {
        isCalibrated = true;
        Serial.println("Calibration Mode is Done ✅");
      }

      // debug output
      Serial.print("Calibrated Humidity: ");
      Serial.print(humidityOffSet);
      Serial.print(" %  |  ");
      Serial.print("Calibrated Temperature: ");
      Serial.print(temperatureOffSet);
      Serial.println(" °C");
    }


    // keep only last N samples
    if (samples.size() > samplesCount + 1) {
        samples.pop();
    }

    delay(200);

}





// corrected getters
float DHTReader::readTemperature() {
  return isCalibrated ? temperatureOffSet : readRawTemperature();
}
float DHTReader::readHumidity() {
  return isCalibrated ? humidityOffSet : readRawHumidity();
}

bool DHTReader::isCalibrat(){
  return isCalibrated;
}


