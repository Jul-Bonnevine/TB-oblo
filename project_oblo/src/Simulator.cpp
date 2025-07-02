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
        float delta = T_forecast - T_mes;
        return T_mes + delta * n * std::exp(-k_m);
    } 
    else 
    {
        return T_mes;
    }
 }
 