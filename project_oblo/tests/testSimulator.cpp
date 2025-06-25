#include "Simulator.h"
#include <iostream>

int main() {
    std::cout << "[SIMULATOR] Test de la température simulée\n";
    std::cout << "------------------------------------------\n";

    float T_mes = 10.0f;
    float T_forecast = 5.0f;

    float T1 = Simulator::computeSimulatedTemperature(T_mes, T_forecast, 1.0f, 0.0f);
    std::cout << "n = 1.0, k_m = 0.0 → T_sim = " << T1 << " °C\n";

    float T2 = Simulator::computeSimulatedTemperature(T_mes, T_forecast, 1.0f, 1.0f);
    std::cout << "n = 1.0, k_m = 1.0 → T_sim = " << T2 << " °C\n";

    float T3 = Simulator::computeSimulatedTemperature(T_mes, T_forecast, 0.5f, 0.5f);
    std::cout << "n = 0.5, k_m = 0.5 → T_sim = " << T3 << " °C\n";

    float T4 = Simulator::computeSimulatedTemperature(T_mes, 12.0f, 1.0f, 1.0f);
    std::cout << "T_forecast > T_mes → T_sim = " << T4 << " °C\n";

    return 0;
}
