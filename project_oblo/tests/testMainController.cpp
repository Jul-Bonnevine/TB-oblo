#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>

// Function for configuration sequence and value reading
float readAndConvertTemperature(MainController& controller, uint8_t channel) {
    controller.getAdc().setChannel(channel);


    if (!controller.getAdc().sendSetup()) {
        std::cerr << "Error SETUP\n";
    }
    usleep(500000);


    if (!controller.getAdc().sendConfig()) {
        std::cerr << "Error CONFIG\n";
    }
    usleep(500000);

    uint16_t raw_adc_value = 0;
    if (!controller.getAdc().readRaw(raw_adc_value)) {
        std::cerr << "[ADC] ADC reading error\n";
        return NAN;
    }

    float tempC = controller.getAdc().readTemperature(raw_adc_value);
    std::cout << "[ADC] Measured temperature : " << tempC << " °C\n";
    return tempC;
}

int main() {
    MainController controller;
    int loop_test = 1;
    int channel_tested = 0;
    while(1)
    {
        std::cout << "============= [TestMainController] test number : " << loop_test << " =============\n";
        std::cout << "==========Testing channel " << channel_tested << "==========\n";
        // 1. read and process ADC values
        float T_mes = readAndConvertTemperature(controller, channel_tested);

        // 2. send measured temperature to API
        if (!controller.getApi().sendTemperature(T_mes)) {
            std::cerr << "[API] Failed to send temperature.\n";
        }
        std::cout << "\n";

        // 3. retrieve weather and settings
        float T_prevu = 0, n = 0, k_m = 0;
        if (!controller.getApi().getForecast(T_prevu)) {
            std::cerr << "[API] Weather forecast recovery error.\n";
        }
        if (!controller.getApi().getParameters(n, k_m)) {
            std::cerr << "[API] Parameter recovery error.\n";
        }

        // 4. Simulated temperature calculation
        float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_prevu, n, k_m);
        std::cout << "[SIM] Simulated temperature:" << T_sim << " °C\n";
        
        // 5. MUX channel conversion and selection
        uint8_t canal = controller.getMultiplexer().convertTemperatureToChannel(T_sim);
        controller.getMultiplexer().selectChannel(canal);

        // 6. Recover NTP time
        std::time_t now = controller.getNtp().getCurrentTime();
        if (now != -1) {
            std::cout << "[NTP] Current time : " << std::ctime(&now);
        } else {
            std::cerr << "[NTP] Time recovery error.\n";
        }
        std::cout << "============= [TestMainController] end of test number : " << loop_test << " =============\n";
        usleep(60000000);
        loop_test = loop_test + 1;

        if (channel_tested > 3) 
        {
            channel_tested = 0;
        } else {
            channel_tested++;
        }
    }
        return 0;
}
