#include "Simulator.h"
#include <iostream>

int main() 
{
    std::cout << "[SIMULATOR] Simulated temperature test\n";
    std::cout << "------------------------------------------\n";

    float T_mes = 10.0f;
    float T_forecast = 5.0f;
    float n = 1.2f; // Example parameter n
    float k_m = 0.0002f; // Example parameter k_m

    float T = Simulator::computeSimulatedTemperature(T_mes, T_forecast, n, k_m);
    std::cout << "T_forecast > T_mes Become T_sim = " << T << " °C\n";

    return 0;
}
