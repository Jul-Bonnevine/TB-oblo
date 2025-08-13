/**
 * @file testMainControllerStateMachine.cpp
 * @brief Implements a state machine for testing MainController operations.
 */

#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cmath>

/** 
* State machine for the main controller
* Each state represents a step in the main controller's operation.
* Initial state is INIT.
* SEND_SETUP state is for sending the setup frame.
* SEND_CONFIG state is for sending the config frame.
* READ_RAW state is for reading the raw ADC value.
* CONVERT_ADC_TEMP state is for converting the raw ADC value to temperature.
* POST_TEMP state is for posting the temperature to the API.
* GET_API_DATA state is for getting data from the API.
* COMPUTE_SIMULATION state is for computing the simulation.
* MUX_CONVERT_TEMP state is for multiplexing and converting the temperature.
* SELECT_CHANNEL state is for selecting the ADC channel.
* GET_TIME state is for getting the current time.
* WAIT_NEXT_CYCLE state is for waiting for the next cycle.
* ERROR state is for handling errors.
*/
enum class MainState {
    INIT,
    SEND_SETUP,
    SEND_CONFIG,
    READ_RAW,
    CONVERT_ADC_TEMP,
    POST_TEMP,
    GET_API_DATA,
    COMPUTE_SIMULATION,
    MUX_CONVERT_TEMP,
    SELECT_CHANNEL,
    GET_TIME,
    WAIT_NEXT_CYCLE,
    ERROR
};

int main() 
{
    MainController controller;

    MainState mainState = MainState::INIT;

    /**
     * Variables for temperature and ADC reading
     * and other parameters used in the simulation.
     */
    float T_mes, T_pred, T_sim;
    float n, k_m;
    uint16_t raw;
    int channel_tested;
    std::time_t now;
    bool stateProcess;

    /**
     * Loop test counter
     */
    int loop_test = 1;
    
    while (true) 
    {
        /**
         * Actions to perform in each state
         */
        switch (mainState) 
        {
            case MainState::INIT:
                if (!controller.getAdc().getSpi().isValid() || !controller.getMultiplexer().getSpi().isValid())
                {
                    std::cerr << "[INIT] SPI init failed.\n";
                    stateProcess = false;
                }
                std::cout << "\n===== [Main Test #" << loop_test << "] =====\n";
                T_mes = NAN, T_pred = NAN, T_sim = NAN;
                n = 0.0f, k_m = 0.0f;
                raw = 0;
                channel_tested = 0;
                now = 0;
                stateProcess = true;
                break;

            case MainState::SEND_SETUP:
                stateProcess = controller.getAdc().sendSetup();
                usleep(10);
                if (!stateProcess)
                {
                    std::cerr << "[ADC] Setup failed.\n";
                }
                break;

            case MainState::SEND_CONFIG:
                stateProcess = controller.getAdc().sendConfig();
                usleep(10);
                if (!stateProcess)
                {
                    std::cerr << "[ADC] Config failed.\n";
                }
                break;

            case MainState::READ_RAW:
                stateProcess = controller.getAdc().readRaw(raw);
                if (!stateProcess)
                {
                    std::cerr << "[ADC] Read raw failed.\n";
                }
                break;

            case MainState::CONVERT_ADC_TEMP:
                T_mes = controller.getAdc().readTemperature(raw);
                std::cout << "[ADC] T_mes = " << T_mes << " °C\n";
                std::cout << "===========================================\n";
                if (std::isnan(T_mes) || T_mes < -15.0f || T_mes > 40.0f) 
                {
                    std::cerr << "[ADC] Temperature invalid.\n";
                    stateProcess = false;
                }
                break;

            case MainState::POST_TEMP:
                stateProcess = controller.getApi().sendTemperature(T_mes);
                if (!stateProcess)
                {
                    std::cerr << "[API] Send temperature failed.\n";
                }
                break;

            case MainState::GET_API_DATA:
                if (!controller.getApi().getForecast(T_pred) || !controller.getApi().getParameters(n, k_m))
                {
                    std::cerr << "[API] Get data failed.\n";
                    stateProcess = false;
                }
                std::cout << "===========================================\n";
                break;

            case MainState::COMPUTE_SIMULATION:
                T_sim = Simulator::computeSimulatedTemperature(T_mes, T_pred, n, k_m);
                std::cout << "[SIM] T_sim = " << T_sim << " °C\n";
                std::cout << "===========================================\n";
                break;

            case MainState::MUX_CONVERT_TEMP:
                channel_tested = controller.getMultiplexer().convertTemperatureToChannel(T_sim);
                std::cout << "[MUX] Channel selected: " << channel_tested << "\n";
                std::cout << "===========================================\n";
                break;

            case MainState::SELECT_CHANNEL:
                stateProcess = controller.getMultiplexer().selectChannel(channel_tested);
                if (!stateProcess)
                {
                    std::cerr << "[MUX] Channel selection failed.\n";
                }
                break;

            case MainState::GET_TIME:
                now = controller.getNtp().getCurrentTime();
                if (now == -1) 
                {
                    std::cerr << "[NTP] Time error.\n";
                    stateProcess = false;
                } else 
                {
                    std::cout << "[NTP] " << std::ctime(&now);
                }
                break;

            case MainState::WAIT_NEXT_CYCLE:
                std::cout << "[LOOP] End of test #" << loop_test << "\n";
                /**
                 * Wait for a while before restarting a new test
                 */
                usleep(60000000);
                loop_test++;
                break;

            case MainState::ERROR:
                std::cerr << "[MAIN] Error occurred, wait 60s then restart.\n";
                /**
                 * Wait for a while before restarting a new test
                 */
                usleep(60000000);
                loop_test++;
                break;
        }

        /**
         * Actions to perform in each state
         */
        switch (mainState) 
        {
            case MainState::INIT:
                if (stateProcess)
                    mainState = MainState::SEND_SETUP;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::SEND_SETUP:
                if (stateProcess)
                    mainState = MainState::SEND_CONFIG;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::SEND_CONFIG:
                if (stateProcess)
                    mainState = MainState::READ_RAW;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::READ_RAW:
                if (stateProcess)
                    mainState = MainState::CONVERT_ADC_TEMP;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::CONVERT_ADC_TEMP:
                if (stateProcess)
                    mainState = MainState::POST_TEMP;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::POST_TEMP:
                if (stateProcess)
                    mainState = MainState::GET_API_DATA;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::GET_API_DATA:
                if (stateProcess)
                    mainState = MainState::COMPUTE_SIMULATION;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::COMPUTE_SIMULATION:
                if (stateProcess)
                    mainState = MainState::MUX_CONVERT_TEMP;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::MUX_CONVERT_TEMP:
                if (stateProcess)
                    mainState = MainState::SELECT_CHANNEL;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::SELECT_CHANNEL:
                if (stateProcess)
                    mainState = MainState::GET_TIME;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::GET_TIME:
                if (stateProcess)
                    mainState = MainState::WAIT_NEXT_CYCLE;
                else
                    mainState = MainState::ERROR;
                break;

            case MainState::WAIT_NEXT_CYCLE:
            case MainState::ERROR:
                mainState = MainState::INIT;
                break;
        }
    }

    return 0;
}
