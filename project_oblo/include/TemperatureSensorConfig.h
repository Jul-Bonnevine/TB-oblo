#ifndef TEMPERATURE_SENSOR_CONFIG_H
#define TEMPERATURE_SENSOR_CONFIG_H

class TemperatureSensorConfig {
public:
    float Vcc;
    float R_fixe;
    float R_fils;
    float beta;
    float R25;
    float T25;

    // Constructeur principal
    TemperatureSensorConfig(float vcc, float rFixe, float rFils,
                            float beta, float r25, float t25);

    // Constructeur par défaut
    TemperatureSensorConfig();
};

#endif // TEMPERATURE_SENSOR_CONFIG_H
