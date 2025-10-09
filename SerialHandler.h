
#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H


class SerialHandler {

  // variable to store the inputs
  private:
    bool monitoringActive;
    float tempThreshold;
    float humidityThreshold;
    float tempThresholdmin;
    float humidityThresholdmin;


  public:

    bool flag = true;
    // constructor
    SerialHandler();  

    // display menu options
    void showMenu();

    // read from Serial and act
    void handleInput();   
    bool isMonitoring(); 

    // getters and setters
    float getTemperatureThreshold();
    float getHumidityThreshold(); 
    void setTemperatureThreshold();
    void setHumidityThreshold();
    bool getMonitoringActive();
    void setMinTemperatureThershold();
    void setMinHumidityThershold();
    float getMinTemperatureThreshold();
    float getMinHumidityThreshold();
    void getMenu();

};

#endif

