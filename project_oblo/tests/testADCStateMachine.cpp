#include "SPI_Interface.h"
#include "ADC.h"
#include "TemperatureSensorConfig.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cstdint>
#include <ctime>

// Définition des états
enum class ADCState {
    SEND_SETUP,
    SEND_CONFIG,
    READ_RAW,
    CONVERT_TEMP,
    DONE,
    ERROR
};

int main() {
    // Initialisation unique
    SPIInterface spi("/dev/spidev1.0", SPI_MODE_0, 8, 500000);
    if (!spi.isValid()) {
        std::cerr << "[INIT] SPI interface invalid.\n";
        return 1;
    }

    TemperatureSensorConfig config(
        3.3f,     // Vcc in volts
        22000.0f, // R_fixed
        3.8f,     // R_wires
        3960.0f,  // Beta
        10000.0f, // R25
        298.15f   // T25 in Kelvin
    );

    ADC adc(spi, config);
    adc.setChannel(0);  // canal fixe

    int loop_count = 1;
    ADCState currentState;
    uint16_t raw = 0;
    float temp = NAN;

    while (true) {
        std::cout << "\n===== [ADC Test #" << loop_count << "] =====\n";

        // Réinitialiser les variables de cycle
        currentState = ADCState::SEND_SETUP;
        raw = 0;
        temp = NAN;

        // === MACHINE D'ÉTAT ===
        while (true) {
            switch (currentState) {
                case ADCState::SEND_SETUP:
                    if (!adc.sendSetup()) {
                        std::cerr << "[SETUP] Error sending setup frame.\n";
                        currentState = ADCState::ERROR;
                    } else {
                        usleep(10);
                        currentState = ADCState::SEND_CONFIG;
                    }
                    break;

                case ADCState::SEND_CONFIG:
                    if (!adc.sendConfig()) {
                        std::cerr << "[CONFIG] Error sending config frame.\n";
                        currentState = ADCState::ERROR;
                    } else {
                        usleep(20);
                        currentState = ADCState::READ_RAW;
                    }
                    break;

                case ADCState::READ_RAW:
                    if (!adc.readRaw(raw)) {
                        std::cerr << "[READ] Error reading ADC value.\n";
                        currentState = ADCState::ERROR;
                    } else {
                        std::cout << "[ADC] Raw value: " << raw << "\n";
                        currentState = ADCState::CONVERT_TEMP;
                    }
                    break;

                case ADCState::CONVERT_TEMP:
                    temp = adc.readTemperature(raw);
                    std::cout << "[TEMP] Measured temperature: " << temp << " °C\n";
                    currentState = ADCState::DONE;
                    break;

                case ADCState::DONE:
                    std::cout << "[DONE] Test complete. Waiting 60 seconds...\n";
                    break;  // sortir pour attendre

                case ADCState::ERROR:
                    std::cerr << "[ERROR] Test failed. Waiting 60 seconds...\n";
                    break;  // sortir pour attendre
            }

            // Sortir de la boucle interne si test terminé ou en erreur
            if (currentState == ADCState::DONE || currentState == ADCState::ERROR) {
                break;
            }
        }

        // Attente avant prochain test
        usleep(60000000); // 60 secondes
        loop_count++;
    }

    return 0;
}
