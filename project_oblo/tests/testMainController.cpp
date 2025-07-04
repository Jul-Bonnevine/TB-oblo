#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>

// Function for configuration sequence and value reading
float readAndConvertTemperature(MainController& controller, uint8_t channel) {
    controller.getAdc().setChannel(channel);

    // Send the setup and configuration frames to the ADC
    if (!controller.getAdc().sendSetup()) {
        std::cerr << "Error SETUP\n";
    }
    usleep(500000);

    // Send the configuration frame to the ADC
    if (!controller.getAdc().sendConfig()) {
        std::cerr << "Error CONFIG\n";
    }
    usleep(500000);

    uint16_t raw_adc_value = 0;

    // Read the raw ADC value
    if (!controller.getAdc().readRaw(raw_adc_value)) {
        std::cerr << "[ADC] ADC reading error\n";
        return NAN;
    }

    //Get the temperature in Celsius from the raw ADC value
    float tempC = controller.getAdc().readTemperature(raw_adc_value);
    std::cout << "[ADC] Measured temperature : " << tempC << " °C\n";
    return tempC;
}

int main() {
    MainController controller;
    int loop_test = 1;
    int channel_tested = 0;

    //Check if SPI interfaces are valid
    if (!(controller.getAdc().getSpi().isValid() && controller.getMultiplexer().getSpi().isValid())) 
    {
        std::cerr << "[MAIN] SPI initialization failed.\n";
        return 1;
    }

    //while loop for testing all the process
    while(1)
    {
        std::cout << "============= [TestMainController] test number : " << loop_test << " =============\n";
        std::cout << "==========Testing channel " << channel_tested << "==========\n";
        // 1. read and process ADC values
        float T_mes = readAndConvertTemperature(controller, channel_tested);

        if(std::isnan(T_mes) || T_mes < -15.0f || T_mes > 40.0f)    
        {
            std::cerr << "[MAIN] Temperature reading error.\n";
        }
        else 
        {
            // 2. send measured temperature to API
            if (!controller.getApi().sendTemperature(T_mes)) 
            {
                std::cerr << "[API] Failed to send temperature.\n";
            }
            std::cout << "\n";

            // 3. retrieve weather and settings
            float T_pred = 0, n = 0, k_m = 0;
            if (!controller.getApi().getForecast(T_pred)) 
            {
                std::cerr << "[API] Weather forecast recovery error.\n";
            }
            if (!controller.getApi().getParameters(n, k_m)) 
            {
                std::cerr << "[API] Parameter recovery error.\n";
            }

            // 4. Simulated temperature calculation
            float T_sim = Simulator::computeSimulatedTemperature(T_mes, T_pred, n, k_m);
            std::cout << "[SIM] Simulated temperature:" << T_sim << " °C\n";
            
            // 5. MUX channel conversion and selection
            uint8_t channel = controller.getMultiplexer().convertTemperatureToChannel(T_sim);
            controller.getMultiplexer().selectChannel(channel);
        }
        

        // 6. Recover NTP time
        std::time_t now = controller.getNtp().getCurrentTime();
        if (now == -1) {
            std::cerr << "[NTP] Time recovery error.\n";
        }
        else 
        {
            std::cout << "[NTP] Current time : " << std::ctime(&now);
        }
        
        std::cout << "============= [TestMainController] end of test number : " << loop_test << " =============\n";

        // Sleep for 60 seconds before the next iteration
        usleep(60000000);
        loop_test = loop_test + 1;

        if (channel_tested >= 3) 
        {
            channel_tested = 0;
        } else {
            channel_tested++;
        }
    }
        return 0;
}
