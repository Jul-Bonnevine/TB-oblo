#include "ObloAPI.h"
#include <iostream>

int main() {
    // Dummy or real MAC to be adjusted
    std::string mac = "0030DEABCDEF";
    ObloAPI api(mac);

    // === Test 1: measured temperature sent ===

    // Simulated temperature to send
    float temp_test = 25.0f;
    std::cout << "[API] Sending T = " << temp_test << " °C...\n";
    if (api.sendTemperature(temp_test)) 
    {
        std::cout << "[API] Temperature sent successfully.\n";
    } else 
    {
        std::cerr << "[API] Error sending temperature.\n";
    }
    std::cout << "------------------------------------\n";

    // === Test 2: retrieve forecast temperature ===
    float T_pred = 0.0f;
    std::cout << "[API] Retrieving forecast temperature...\n";
    if (api.getForecast(T_pred)) 
    {
        std::cout << "[API] Forecast temperature = " << T_pred << " °C\n";
    } else 
    {
        std::cerr << "[API] Error retrieving forecast temperature.\n";
    }

    std::cout << "------------------------------------\n";

    // === Test 3: retrieve parameters n and k_m ===
    float n = 0.0f, k_m = 0.0f;
    std::cout << "[API] Retrieving parameters n and k_m...\n";
    if (api.getParameters(n, k_m)) 
    {
        std::cout << "[API] Parameters received: n = " << n << ", k_m = " << k_m << "\n";
    } else 
    {
        std::cerr << "[API] Error retrieving parameters.\n";
    }

    return 0;
}
