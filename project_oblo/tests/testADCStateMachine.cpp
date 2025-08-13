/**
 * @file testADCStateMachine.cpp
 * @brief Implements a state machine for testing the ADC (Analog-to-Digital Converter) operations.
 */

#include "SPI_Interface.h"
#include "ADC.h"
#include "TemperatureSensorConfig.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cstdint>
#include <ctime>

/**
 * State machine for ADC operations.
 * Each state represents a step in the ADC reading process.
 * Initial state is INIT.
 * SEND_SETUP state is for sending the setup frame.
 * SEND_CONFIG state is for sending the config frame.
 * READ_RAW state is for reading the raw ADC value.
 * CONVERT_TEMP state is for converting the raw ADC value to temperature.
 * DONE state is for indicating that the ADC reading process is complete.
 * ERROR state is for handling errors during the ADC reading process.
 */
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

    /**
     * Initialisation SPI for ADC
     * The ADC is on SPI1
     * Configuration: Mode 0, 8 bits, 500kHz
     */
    SPIInterface spi("/dev/spidev1.0", SPI_MODE_0, 8, 500000);
    if (!spi.isValid()) 
    {
        std::cerr << "[INIT] SPI interface invalid.\n";
        return 1;
    }

    /**
     * Probe acquisition setup
     */
    TemperatureSensorConfig config
    (
        3.3f,     // Vcc
        22000.0f, // R_fixed
        3.8f,     // R_wires
        3977.0f,  // Beta
        10000.0f, // R25
        298.15f   // T25 (K)
    );

    /**
     * ADC initialisation
     * SPI interface and configuration parameters
     */
    ADC adc(spi, config);

    /**
     * Set ADC channel to 0
     */
    adc.setChannel(0);

    /**
     * Loop counter
     */
    int loop_count = 1;

    ADCState currentState = ADCState::INIT;
    bool operationSuccess;
    uint16_t raw;
    float temp;

    while (true) 
    {
        /**
         * Actions to perform in each state
         */
        switch (currentState) 
        {
            case ADCState::INIT:
                std::cout << "\n===== [ADC Test #" << loop_count << "] =====\n";
                raw = 0;
                temp = NAN;
                operationSuccess = true;
                break;

            case ADCState::SEND_SETUP:
                operationSuccess = adc.sendSetup();
                if (!operationSuccess)
                {
                    std::cerr << "[SETUP] Error sending setup frame.\n";
                }                    
                usleep(10);
                break;

            case ADCState::SEND_CONFIG:
                operationSuccess = adc.sendConfig();
                if (!operationSuccess)
                {
                    std::cerr << "[CONFIG] Error sending config frame.\n";
                }
                usleep(10);
                break;

            case ADCState::READ_RAW:
                operationSuccess = adc.readRaw(raw);
                if (operationSuccess) 
                {
                    std::cout << "[ADC] Raw value: " << raw << "\n";
                } else 
                {
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

        /**
         * Actions to perform in each state
         */
        switch (currentState) 
        {
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
                if (temp < -15.0f || temp > 40.0f) 
                {
                    std::cerr << "[TEMP] Invalid temperature: skipping cycle\n";
                    currentState = ADCState::ERROR;
                } else 
                {
                    currentState = ADCState::DONE;
                }
                break;

            case ADCState::DONE:
                /**
                * Wait for a while before restarting a new test
                */
                usleep(6000000);
                currentState = ADCState::INIT;
                loop_count++;
                break;
                
            case ADCState::ERROR:
                /**
                * Wait for a while before restarting a new test
                */
                usleep(6000000);
                currentState = ADCState::INIT;
                loop_count++;
                break;
        }
    }

    return 0;
}
