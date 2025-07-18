#include "SPI_Interface.h"
#include "ADC.h"
#include "TemperatureSensorConfig.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cstdint>
#include <ctime>

// États possibles de la FSM
enum class ADCState {
    INIT,
    SEND_SETUP,
    SEND_CONFIG,
    READ_RAW,
    CONVERT_TEMP,
    DONE,
    ERROR
};

int main() {
    // Initialisation SPI
    SPIInterface spi("/dev/spidev1.0", SPI_MODE_0, 8, 500000);
    if (!spi.isValid()) {
        std::cerr << "[INIT] SPI interface invalid.\n";
        return 1;
    }

    // Configuration de la sonde
    TemperatureSensorConfig config(
        3.3f,     // Vcc
        22000.0f, // R_fixed
        3.8f,     // R_wires
        3977.0f,  // Beta
        10000.0f, // R25
        298.15f   // T25 (K)
    );

    ADC adc(spi, config);
    adc.setChannel(0);  // Canal fixe

    int loop_count = 1;
    ADCState currentState = ADCState::INIT;
    bool operationSuccess = false;
    uint16_t raw = 0;
    float temp = NAN;

    while (true) {
        // === ACTIONS ===
        switch (currentState) {
            case ADCState::INIT:
                std::cout << "\n===== [ADC Test #" << loop_count << "] =====\n";
                raw = 0;
                temp = NAN;
                operationSuccess = true;  // INIT always successful
                break;

            case ADCState::SEND_SETUP:
                operationSuccess = adc.sendSetup();
                if (!operationSuccess)
                    std::cerr << "[SETUP] Error sending setup frame.\n";
                usleep(10);
                break;

            case ADCState::SEND_CONFIG:
                operationSuccess = adc.sendConfig();
                if (!operationSuccess)
                    std::cerr << "[CONFIG] Error sending config frame.\n";
                usleep(20);
                break;

            case ADCState::READ_RAW:
                operationSuccess = adc.readRaw(raw);
                if (operationSuccess) {
                    std::cout << "[ADC] Raw value: " << raw << "\n";
                } else {
                    std::cerr << "[READ] Error reading ADC value.\n";
                }
                break;

            case ADCState::CONVERT_TEMP:
                temp = adc.readTemperature(raw);
                std::cout << "[TEMP] Measured temperature: " << temp << " °C\n";
                operationSuccess = true;
                break;

            case ADCState::DONE:
                std::cout << "[DONE] Test complete. Waiting 60 seconds...\n";
                break;

            case ADCState::ERROR:
                std::cerr << "[ERROR] Test failed. Waiting 60 seconds...\n";
                break;
        }

        // === TRANSITIONS ===
        switch (currentState) {
            case ADCState::INIT:
                currentState = ADCState::SEND_SETUP;
                break;

            case ADCState::SEND_SETUP:
                if (operationSuccess)
                    currentState = ADCState::SEND_CONFIG;
                else
                    currentState = ADCState::ERROR;
                break;

            case ADCState::SEND_CONFIG:
                if (operationSuccess)
                    currentState = ADCState::READ_RAW;
                else
                    currentState = ADCState::ERROR;
                break;

            case ADCState::READ_RAW:
                if (operationSuccess)
                    currentState = ADCState::CONVERT_TEMP;
                else
                    currentState = ADCState::ERROR;
                break;

            case ADCState::CONVERT_TEMP:
                if (std::isnan(temp) || temp < -15.0f || temp > 40.0f) {
                    std::cerr << "[TEMP] Invalid temperature: skipping cycle\n";
                    currentState = ADCState::ERROR;
                } else {
                    currentState = ADCState::DONE;
                }
                break;

            case ADCState::DONE:
            case ADCState::ERROR:
                usleep(6000000);  // 60 sec pause entre chaque test
                currentState = ADCState::INIT;
                loop_count++;
                break;
        }
    }

    return 0;
}
