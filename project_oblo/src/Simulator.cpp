/**
 * @file Simulator.cpp
 * @brief Implements the Simulator class used to compute simulated temperature values.
 */
#include "Simulator.h"
#include <cmath>

 float Simulator::computeSimulatedTemperature(float T_mes, float T_forecast, float n, float k_m)
 {
    if (T_forecast < T_mes)     //change signe of the condition
    {
        /**
        * Formula for simulated temperature.
        * This formula is given by the Oblo engineering team.
        */
        return T_mes + (T_forecast - T_mes) * n * std::exp(-k_m);
    } 
    else 
    {
        return T_mes;
    }
 }
 