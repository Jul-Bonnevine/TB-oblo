#include "MainController.h"



MainController::MainController()
    : adcSpi("/dev/spidev1.0", SPI_MODE_0, 8, 500000),
      muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000),
      adc(adcSpi),
      mux(muxSpi),
      api("0030DEABCDEF"),
      ntp("time.google.com") 
{

}

void MainController::run()
{   
    // === [1] Heure NTP ===
    std::time_t now = ntp.getCurrentTime();
    if (now == -1) {
        std::cerr << "Erreur lors de la récupération de l'heure NTP.\n";
    } else {
        std::cout << "Heure NTP : " << std::ctime(&now);
    }

    // === [2] Température mesurée (temporaire en attendant ADC) ===
    float T_mes = 28.3f;
    if (!api.sendTemperature(T_mes)) {
        std::cerr << "Échec de l'envoi de la température.\n";
    } else {
        std::cout << "Température envoyée avec succès.\n";
    }

    // === [3] Récupération météo prévue ===
    float T_prevu = 0;
    if (!api.getForecast(T_prevu)) {
        std::cerr << "Erreur de récupération météo.\n";
        return;
    } else {
        std::cout << "Météo prévue : " << T_prevu << " °C\n";
    }

    // === [4] Récupération des paramètres de simulation ===
    float n = 0, k_m = 0;
    if (!api.getParameters(n, k_m)) {
        std::cerr << "Erreur de récupération des paramètres.\n";
        return;
    } else {
        std::cout << "Paramètres reçus : n = " << n << ", k_m = " << k_m << "\n";
    }

    // === [5] Calcul de température simulée ===
    float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_prevu, n, k_m);
    std::cout << "Température simulée : " << T_sim << " °C\n";

    // === [6] Sélection de la température à simuler ===
    float T_utilisee = (T_prevu > T_mes) ? T_sim : T_mes;
    std::cout << "Température utilisée pour le multiplexeur : " << T_utilisee << " °C\n";

    // === [7] Conversion température -> canal ===
    uint8_t canal = mux.convertTemperatureToChannel(T_utilisee);
    std::cout << "Canal calculé : " << static_cast<int>(canal) << "\n";

    // === [8] Envoi de la trame SPI au multiplexeur ===
    if (!mux.selectChannel(canal)) {
        std::cerr << "Erreur lors de l'envoi au multiplexeur.\n";
    } else {
        std::cout << "Canal sélectionné avec succès.\n";
    }
}

void MainController::processOneCycle(uint8_t adc_channel)
{
    
}