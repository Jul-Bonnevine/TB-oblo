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
    float T_mes = 22.5f;
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
}

void MainController::processOneCycle(uint8_t adc_channel)
{
    
}