#include "TemperatureSensorConfig.h"

// Constructeur avec paramètres
TemperatureSensorConfig::TemperatureSensorConfig(float vcc, float rFixe, float rFils,
                                                 float beta, float r25, float t25)
    : Vcc(vcc), R_fixe(rFixe), R_fils(rFils), beta(beta), R25(r25), T25(t25)
{}

// Constructeur par défaut
TemperatureSensorConfig::TemperatureSensorConfig()
    : Vcc(0.0f), R_fixe(0.0f), R_fils(0.0f), beta(0.0f), R25(0.0f), T25(0.0f)
{}

// Getters
float TemperatureSensorConfig::getVcc() const    { return Vcc; }
float TemperatureSensorConfig::getRFixe() const  { return R_fixe; }
float TemperatureSensorConfig::getRFils() const  { return R_fils; }
float TemperatureSensorConfig::getBeta() const   { return beta; }
float TemperatureSensorConfig::getR25() const    { return R25; }
float TemperatureSensorConfig::getT25() const    { return T25; }