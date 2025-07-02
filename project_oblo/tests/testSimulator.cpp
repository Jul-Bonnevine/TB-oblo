#include "Simulator.h"
#include <iostream>

int main() {
    std::cout << "[SIMULATOR] Simulated temperature test\n";
    std::cout << "------------------------------------------\n";

    float T_mes = 10.0f;
    float T_forecast = 5.0f;


    float T = Simulator::computeSimulatedTemperature(T_mes, 12.0f, 1.0f, 1.0f);
    std::cout << "T_forecast > T_mes Become T_sim = " << T << " °C\n";

    return 0;
}
