#ifndef TEMPERATURE_SENSOR_CONFIG_H
#define TEMPERATURE_SENSOR_CONFIG_H

class TemperatureSensorConfig {
public:
    
    // Constructeur principal
    TemperatureSensorConfig(float vcc, float rFixe, float rFils,
                            float beta, float r25, float t25);

    // Constructeur par défaut
    TemperatureSensorConfig();

    // Getters
    float getVcc() const;
    float getRFixe() const;
    float getRFils() const;
    float getBeta() const;
    float getR25() const;
    float getT25() const;

private:
    float Vcc;
    float R_fixe;
    float R_fils;
    float beta;
    float R25;
    float T25;

};

#endif // TEMPERATURE_SENSOR_CONFIG_H
