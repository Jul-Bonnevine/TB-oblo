#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cmath>

// États possibles de la machine
enum class TestState {
    READ_ADC,
    POST_TEMP,
    GET_API_DATA,
    COMPUTE_SIMULATION,
    SELECT_MUX_CHANNEL,
    GET_TIME,
    WAIT_NEXT_CYCLE
};

int main() {
    MainController controller;

    // Vérification des interfaces SPI
    if (!(controller.getAdc().getSpi().isValid() && controller.getMultiplexer().getSpi().isValid())) {
        std::cerr << "[MAIN] SPI initialization failed.\n";
        return 1;
    }

    TestState currentState = TestState::READ_ADC;
    float T_mes = NAN, T_pred = NAN, T_sim = NAN;
    float n = 0.0f, k_m = 0.0f;
    std::time_t now = 0;
    int loop_test = 1;
    int channel_tested = 0;

    while (true) {
        std::cout << "\n===== [Test #" << loop_test << "] Channel " << channel_tested << " =====\n";

        // === ACTIONS ===
        switch (currentState) {
            case TestState::READ_ADC: {
                controller.getAdc().setChannel(channel_tested);

                if (!controller.getAdc().sendSetup()) {
                    std::cerr << "[ADC] Error SETUP\n";
                }
                usleep(500000);

                if (!controller.getAdc().sendConfig()) {
                    std::cerr << "[ADC] Error CONFIG\n";
                }
                usleep(500000);

                uint16_t raw = 0;
                if (controller.getAdc().readRaw(raw)) {
                    T_mes = controller.getAdc().readTemperature(raw);
                    std::cout << "[ADC] T_mes = " << T_mes << " °C\n";
                } else {
                    std::cerr << "[ADC] Error reading value\n";
                    T_mes = NAN;
                }
                break;
            }

            case TestState::POST_TEMP:
                if (!controller.getApi().sendTemperature(T_mes)) {
                    std::cerr << "[API] Failed to send temperature\n";
                }
                break;

            case TestState::GET_API_DATA:
                if (!controller.getApi().getForecast(T_pred)) {
                    std::cerr << "[API] Forecast error\n";
                }
                if (!controller.getApi().getParameters(n, k_m)) {
                    std::cerr << "[API] Parameters error\n";
                }
                break;

            case TestState::COMPUTE_SIMULATION:
                T_sim = Simulator::computeSimulatedTemperature(T_mes, T_pred, n, k_m);
                std::cout << "[SIM] T_sim = " << T_sim << " °C\n";
                break;

            case TestState::SELECT_MUX_CHANNEL: {
                uint8_t channel = controller.getMultiplexer().convertTemperatureToChannel(T_sim);
                if (!controller.getMultiplexer().selectChannel(channel)) {
                    std::cerr << "[MUX] Channel selection failed\n";
                }
                break;
            }

            case TestState::GET_TIME:
                now = controller.getNtp().getCurrentTime();
                if (now == -1) {
                    std::cerr << "[NTP] Time recovery error\n";
                } else {
                    std::cout << "[NTP] Current time: " << std::ctime(&now);
                }
                break;

            case TestState::WAIT_NEXT_CYCLE:
                std::cout << "[MAIN] End of test #" << loop_test << "\n";
                usleep(60000000);  // Pause 60s
                loop_test++;
                break;
        }

        // === TRANSITIONS ===
        switch (currentState) {
            case TestState::READ_ADC:
                if (std::isnan(T_mes) || T_mes < -15.0f || T_mes > 40.0f) {
                    std::cerr << "[MAIN] Invalid temperature: skipping to next cycle\n";
                    currentState = TestState::WAIT_NEXT_CYCLE;
                } else {
                    currentState = TestState::POST_TEMP;
                }
                break;

            case TestState::POST_TEMP:
                currentState = TestState::GET_API_DATA;
                break;

            case TestState::GET_API_DATA:
                currentState = TestState::COMPUTE_SIMULATION;
                break;

            case TestState::COMPUTE_SIMULATION:
                currentState = TestState::SELECT_MUX_CHANNEL;
                break;

            case TestState::SELECT_MUX_CHANNEL:
                currentState = TestState::GET_TIME;
                break;

            case TestState::GET_TIME:
                currentState = TestState::WAIT_NEXT_CYCLE;
                break;

            case TestState::WAIT_NEXT_CYCLE:
                currentState = TestState::READ_ADC;
                break;
        }
    }

    return 0;
}
