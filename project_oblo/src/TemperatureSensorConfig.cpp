/**
 * @file TemperatureSensorConfig.cpp
 * @brief Implements the TemperatureSensorConfig class for storing thermistor parameters.
 */
#include "TemperatureSensorConfig.h"

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


