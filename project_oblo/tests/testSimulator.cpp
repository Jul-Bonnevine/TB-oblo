/**
 * @file testSimulator.cpp
 * @brief Implements a while true loop for testing the Simulator operations.
 */

#include "Simulator.h"
#include <iostream>

int main() 
{
    std::cout << "[SIMULATOR] Simulated temperature test\n";
    std::cout << "------------------------------------------\n";

    /**
     * Simulated temperature values
     * T_mes: Measured temperature
     * T_forecast: Forecasted temperature
     * n, k_m: Parameters for the simulation
     */
    float T_mes = 31.0f;
    float T_forecast = 29.6f;
    float n = 1.2f; 
    float k_m = 0.0002f; 

    float T = Simulator::computeSimulatedTemperature(T_mes, T_forecast, n, k_m);
    std::cout << "T_forecast > T_mes Become T_sim = " << T << " °C\n";

    return 0;
}
