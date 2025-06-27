#include "MainController.h"
#include <iostream>
#include <unistd.h>

void processOneCycle(MainController& controller, uint8_t adc_channel) {
    controller.getAdc().setChannel(adc_channel);

    for (int i = 0; i < 5; ++i) {
        if (!controller.getAdc().sendSetup()) {
            std::cerr << "Erreur SETUP\n";
        }
        usleep(500000);
    }

    for (int i = 0; i < 5; ++i) {
        if (!controller.getAdc().sendConfig()) {
            std::cerr << "Erreur CONFIG\n";
        }
        usleep(500000);
    }

    usleep(10);

    float T_mes = controller.getAdc().readTemperature();
    std::cout << "[ADC] Température mesurée : " << T_mes << " °C\n";

    if (!controller.getApi().sendTemperature(T_mes)) {
        std::cerr << "[API] Échec envoi température.\n";
    }
}

int main() {
    MainController controller;

    bool modeManuel = true;  // ← Passe à true pour tester une valeur manuelle
    float T_mes = 0.0f;

    if (modeManuel) {
        T_mes = 7.5f;  // Valeur fictive pour test
        std::cout << "[TEST] Mode manuel : T_mes = " << T_mes << " °C\n";
    } else {
        processOneCycle(controller, 0);  // Trames visibles à l'oscillo
        T_mes = controller.getAdc().readTemperature();
        std::cout << "[TEST] Température réelle mesurée : " << T_mes << " °C\n";
    }

    // Envoi température mesurée
    controller.getApi().sendTemperature(T_mes);

    // Heure NTP
    std::time_t now = controller.getNtp().getCurrentTime();
    if (now != -1) {
        std::cout << "[TEST] Heure NTP : " << std::ctime(&now);
    }

    // Récupération météo et paramètres
    float T_prevu = 0, n = 0, k_m = 0;
    controller.getApi().getForecast(T_prevu);
    controller.getApi().getParameters(n, k_m);

    // Calcul température simulée
    float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_prevu, n, k_m);
    float T_utilisee = (T_prevu > T_mes) ? T_sim : T_mes;

    // Envoi au MUX
    uint8_t canal = controller.getMultiplexer().convertTemperatureToChannel(T_utilisee);
    controller.getMultiplexer().selectChannel(canal);

    for (int i = 0; i < 2; ++i) {
        controller.getMultiplexer().selectChannel(canal);
        usleep(500000);
    }

    return 0;
}
