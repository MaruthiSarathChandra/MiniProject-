
#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H


class SerialHandler {

  // variable to store the inputs
  private:
    bool monitoringActive;
    float tempThreshold;
    float humidityThreshold;


  public:
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

};

#endif

