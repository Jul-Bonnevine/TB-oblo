#ifndef TEMPERATURE_SENSOR_CONFIG_H
#define TEMPERATURE_SENSOR_CONFIG_H

class TemperatureSensorConfig {
public:
    
    //Default constructorm set all probe parameters to 0
    TemperatureSensorConfig();

    //Constructor with probe parameters to initialise them in the mainController
    TemperatureSensorConfig(float vcc, float rFixed, float rWires, float beta, float r25, float t25);

    /*
    Getters attributes of the TemperatureSensorConfig class.
    This allows values to be modified outside this class
    */   
    float getVcc() const{ return Vcc; };
    float getRFixed() const { return R_fixed; };
    float getRWires() const { return R_wires; };
    float getBeta() const { return Beta; };
    float getR25() const { return R25; };
    float getT25() const { return T25; };

private:

    //Probe attributes
    float Vcc;
    float R_fixed;
    float R_wires;
    float Beta;
    float R25;
    float T25;

};

#endif
