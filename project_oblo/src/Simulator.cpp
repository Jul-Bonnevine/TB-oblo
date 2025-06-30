#include "Simulator.h"

#include <cmath>

 float Simulator::computeSimulatedTemperature(float T_mes, float T_forecast, float n, float k_m)
 {
    if (T_forecast < T_mes)     //modification du signe pour pouvoir tester réelement
    {
        float delta = T_forecast - T_mes;
        return T_mes + delta * n * std::exp(-k_m);
    } 
    else 
    {
        return T_mes;
    }
 }
 