#include "MainController.h"



MainController::MainController()
    : adcSpi("/dev/spidev1.0", SPI_MODE_0, 8, 500000),
      muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000),
      adc(adcSpi, 
          3.3f,    // Vcc en volts
          2000.0f, // R_fixe en ohms
          3.8f,    // R_fils en ohms
          3950.0f, // Beta
          1000.0f, // R25 en ohms
          298.15f  // T25 en Kelvin
      ),
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
    float T_mes = 8.5f;
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

        // === [9] Boucle d'envoi SPI pour oscilloscope ===
    std::cout << "Envoi périodique sur le SPI pour capture oscilloscope...\n";
    for (int i = 0; i < 2; ++i) {  // Envoi 10 fois (ou while(true) si tu veux manuel)
        if (!mux.selectChannel(canal)) {
            std::cerr << "Erreur lors de l'envoi SPI.\n";
        } else {
            std::cout << "Canal " << static_cast<int>(canal) 
                      << " envoyé (itération " << i + 1 << ").\n";
        }
        usleep(500000); // 500 ms
    }

    // === [10] Lecture du canal 0 de l'ADC
    processOneCycle(2);  // Lecture canal 2

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