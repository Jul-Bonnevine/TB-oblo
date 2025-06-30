#ifndef TEMPERATURE_SENSOR_CONFIG_H
#define TEMPERATURE_SENSOR_CONFIG_H

class TemperatureSensorConfig {
public:
    
    // Constructeur principal
    TemperatureSensorConfig(float vcc, float rFixe, float rFils, float beta, float r25, float t25);

    // Constructeur par défaut
    TemperatureSensorConfig();

    /*
    Getters des attributs de la classe TemperatureSensorConfig.
    Cela permet de modifier les valeurs en dehors de cette classe
    */   
    float getVcc() const{ return Vcc; };
    float getRFixe() const { return R_fixe; };
    float getRFils() const { return R_fils; };
    float getBeta() const { return Beta; };
    float getR25() const { return R25; };
    float getT25() const { return T25; };

private:

    //Attributs de la sonde
    float Vcc;
    float R_fixe;
    float R_fils;
    float Beta;
    float R25;
    float T25;

};

#endif
