#include "MainController.h"
#include <iostream>
#include <unistd.h>

int main() {
    MainController controller;

    // === [1] Heure NTP ===
    std::time_t now = controller.getNtp().getCurrentTime();
    if (now == -1) {
        std::cerr << "[TEST] Erreur lors de la récupération de l'heure NTP.\n";
    } else {
        std::cout << "[TEST] Heure NTP : " << std::ctime(&now);
    }

    // === [2] Température mesurée (ex. fixe pour test) ===
    float T_mes = 7.5f;
    if (!controller.getApi().sendTemperature(T_mes)) {
        std::cerr << "[TEST] Échec de l'envoi de la température.\n";
    } else {
        std::cout << "[TEST] Température envoyée avec succès.\n";
    }

    // === [3] Récupération météo prévue ===
    float T_prevu = 0;
    if (!controller.getApi().getForecast(T_prevu)) {
        std::cerr << "[TEST] Erreur récupération météo.\n";
        return 1;
    }

    // === [4] Paramètres de simulation ===
    float n = 0, k_m = 0;
    if (!controller.getApi().getParameters(n, k_m)) {
        std::cerr << "[TEST] Erreur récupération des paramètres.\n";
        return 1;
    }

    // === [5] Calcul température simulée ===
    float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_prevu, n, k_m);
    float T_utilisee = (T_prevu > T_mes) ? T_sim : T_mes;
    std::cout << "[TEST] T utilisée pour MUX : " << T_utilisee << " °C\n";

    // === [6] Conversion et envoi MUX ===
    uint8_t canal = controller.getMultiplexer().convertTemperatureToChannel(T_utilisee);
    controller.getMultiplexer().selectChannel(canal);

    // === [7] Répétition d'envoi pour l'oscilloscope
    for (int i = 0; i < 2; ++i) {
        controller.getMultiplexer().selectChannel(canal);
        usleep(500000);
    }

    // === [8] Lecture ADC + envoi API
    processOneCycle(0);  // lit canal 0
    return 0;
}

void MainController::processOneCycle(uint8_t adc_channel)
{
    adc.setChannel(adc_channel);

    for (int i = 0; i < 5; ++i) {  // ou while(true) pour manuel
    if (!adc.sendSetup()) {
        std::cerr << "Erreur lors de l'envoi de la trame SETUP.\n";
    } else {
        std::cout << "Trame SETUP envoyée (itération " << i + 1 << ").\n";
    }

    usleep(500000); // 500 ms pour avoir de l'espace sur l'oscillo
    }

    for (int i = 0; i < 5; ++i) {
    if (!adc.sendConfig()) {
        std::cerr << "Erreur lors de l'envoi de la trame CONFIG.\n";
    } else {
        std::cout << "Trame CONFIG envoyée (itération " << i + 1 << ").\n";
    }

    usleep(500000); // 500 ms de délai pour espacer les trames
    }

    usleep(10); // µs pour acquisition/conversion

    float T_mes = adc.readTemperature();
    std::cout << "[ADC] Température mesurée sur canal "
              << static_cast<int>(adc_channel) << " : " 
              << T_mes << " °C\n";

    // Exemple d'envoi vers API
    if (!api.sendTemperature(T_mes)) {
        std::cerr << "[API] Échec de l’envoi de température.\n";
    } else {
        std::cout << "[API] Température envoyée avec succès.\n";
    }    
}
