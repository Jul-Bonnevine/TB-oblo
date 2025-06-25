#include "ObloAPI.h"
#include <iostream>

int main() {
    // MAC fictive ou réelle à ajuster
    std::string mac = "0030DEABCDEF";
    ObloAPI api(mac);

    // === Test 1 : envoi de température mesurée ===
    float temp_test = 12.5f;
    std::cout << "[API] Envoi de T = " << temp_test << " °C...\n";
    if (api.sendTemperature(temp_test)) {
        std::cout << "[API] Température envoyée avec succès.\n";
    } else {
        std::cerr << "[API] Échec de l'envoi de température.\n";
    }

    std::cout << "------------------------------------\n";

    // === Test 2 : récupération de la température prévue ===
    float T_prevu = 0.0f;
    std::cout << "[API] Récupération de la température prévue...\n";
    if (api.getForecast(T_prevu)) {
        std::cout << "[API] Température prévue = " << T_prevu << " °C\n";
    } else {
        std::cerr << "[API] Erreur lors de la récupération météo.\n";
    }

    std::cout << "------------------------------------\n";

    // === Test 3 : récupération des paramètres n et k_m ===
    float n = 0.0f, k_m = 0.0f;
    std::cout << "[API] Récupération des paramètres n et k_m...\n";
    if (api.getParameters(n, k_m)) {
        std::cout << "[API] Paramètres reçus : n = " << n << ", k_m = " << k_m << "\n";
    } else {
        std::cerr << "[API] Erreur de récupération des paramètres.\n";
    }

    return 0;
}
