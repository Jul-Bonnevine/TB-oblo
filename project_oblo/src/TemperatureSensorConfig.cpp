#include "TemperatureSensorConfig.h"

// Constructeur par défaut
TemperatureSensorConfig::TemperatureSensorConfig()   
{
    Vcc = 0.0f;
    R_fixe = 0.0f;
    R_fils = 0.0f;
    Beta = 0.0f;
    R25 = 0.0f;
    T25 = 0.0f;
}

// Constructeur avec paramètres
TemperatureSensorConfig::TemperatureSensorConfig(float vcc, float rFixe, float rFils,
                                                 float beta, float r25, float t25)
{
    Vcc = vcc;
    R_fixe = rFixe;
    R_fils = rFils;
    Beta = beta;
    R25 = r25;
    T25 = t25;
}


