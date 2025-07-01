#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>

// Fonction pour La séquence de configuratuon et la lecture de valeur
float readAndConvertTemperature(MainController& controller, uint8_t channel) {
    controller.getAdc().setChannel(channel);


    if (!controller.getAdc().sendSetup()) {
        std::cerr << "Erreur SETUP\n";
    }
    usleep(500000);


    if (!controller.getAdc().sendConfig()) {
        std::cerr << "Erreur CONFIG\n";
    }
    usleep(500000);

    uint16_t raw_adc_value = 0;
    if (!controller.getAdc().readRaw(raw_adc_value)) {
        std::cerr << "[ADC] Erreur lecture ADC\n";
        return NAN;
    }

    float tempC = controller.getAdc().readTemperature(raw_adc_value);
    std::cout << "[ADC] Température mesurée : " << tempC << " °C\n";
    return tempC;
}

int main() {
    MainController controller;

    // 1. Lire et traiter les valeurs de l’ADC
    float T_mes = readAndConvertTemperature(controller, 0);

    // 2. Envoi de la température mesurée à l'API
    if (!controller.getApi().sendTemperature(T_mes)) {
        std::cerr << "[API] Échec envoi température.\n";
    }

    std::cout << "\n";

    // 3. Récupérer météo et paramètres
    float T_prevu = 0, n = 0, k_m = 0;
    if (!controller.getApi().getForecast(T_prevu)) {
        std::cerr << "[API] Erreur récupération prévision météo.\n";
    }
    if (!controller.getApi().getParameters(n, k_m)) {
        std::cerr << "[API] Erreur récupération paramètres.\n";
    }

    // 4. Calcul température simulée
    float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_prevu, n, k_m);
    
    // 5. Conversion en canal MUX et sélection
    uint8_t canal = controller.getMultiplexer().convertTemperatureToChannel(T_sim);
    controller.getMultiplexer().selectChannel(canal);

    // 6. Récupérer l'heure NTP
    std::time_t now = controller.getNtp().getCurrentTime();
    if (now != -1) {
        std::cout << "[NTP] Heure actuelle : " << std::ctime(&now);
    } else {
        std::cerr << "[NTP] Erreur récupération heure.\n";
    }

    usleep(500000);
    

    return 0;
}
