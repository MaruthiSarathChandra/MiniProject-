#ifndef DHTREADER_H
#define DHTREADER_H

#include <DHT.h>
#include <queue>
#include <utility> // for std::pair

#define DHTPIN 4        // GPIO pin where your DHT sensor is connected
#define DHTTYPE DHT11   // Or DHT11, depending on your sensor


class DHTReader {
private:
    DHT dht;
    float temperatureOffSet;
    float humidityOffSet;
    bool isCalibrated;

    std::queue<std::pair<float, float>> samples;

public:
    DHTReader();

    void begin();
    float readRawTemperature();
    float readRawHumidity();

    void calibrate();

    float readTemperature();
    float readHumidity();
};

#endif
