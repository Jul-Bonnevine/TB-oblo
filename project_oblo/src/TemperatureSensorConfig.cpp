#include "TemperatureSensorConfig.h"

TemperatureSensorConfig::TemperatureSensorConfig()   
{
    Vcc = 0.0f;
    R_fixed = 0.0f;
    R_wires = 0.0f;
    Beta = 0.0f;
    R25 = 0.0f;
    T25 = 0.0f;
}

TemperatureSensorConfig::TemperatureSensorConfig(float vcc, float rFixed, float rWires,
                                                 float beta, float r25, float t25)
{
    Vcc = vcc;
    R_fixed = rFixed;
    R_wires = rWires;
    Beta = beta;
    R25 = r25;
    T25 = t25;
}


