#include "SPI_Interface.h"
#include "AnalogMultiplexer.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstdint>

// États de la machine
enum class MuxTestState {
    INIT,
    CONVERT_TEMP,
    SELECT_CHANNEL,
    WAIT_OSCILLO,
    NEXT_TEMPERATURE,
    FINISHED,
    ERROR
};

int main() {
    SPIInterface muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000);
    if (!muxSpi.isValid()) {
        std::cerr << "[MUX-TEST] Invalid SPI interface.\n";
        return 1;
    }

    AnalogMultiplexer mux(muxSpi);

    std::vector<float> temperatures = {
        -20.0f, -15.0f, -10.0f, 0.0f, 7.5f, 15.0f, 17.5f, 25.0f, 30.0f, 31.5f
    };

    size_t index = 0;
    float currentTemp = 0.0f;
    uint8_t currentChannel = 0;
    bool selectionSuccess = true;

    MuxTestState currentState = MuxTestState::INIT;

    while (true) {
        // === ACTIONS ===
        switch (currentState) {
            case MuxTestState::INIT:
                std::cout << "[MUX-TEST] Initialization OK\n";
                break;

            case MuxTestState::CONVERT_TEMP:
                currentTemp = temperatures[index];
                currentChannel = mux.convertTemperatureToChannel(currentTemp);
                std::cout << "[MUX-TEST] T = " << currentTemp << " °C → channel " << static_cast<int>(currentChannel) << "\n";
                break;

            case MuxTestState::SELECT_CHANNEL:
                selectionSuccess = mux.selectChannel(currentChannel);
                if (!selectionSuccess) {
                    std::cerr << "[MUX-TEST] SPI failure on channel " << static_cast<int>(currentChannel) << "\n";
                }
                break;

            case MuxTestState::WAIT_OSCILLO:
                usleep(500000); // 500 ms pour oscilloscope
                break;

            case MuxTestState::NEXT_TEMPERATURE:
                // rien à faire ici
                break;

            case MuxTestState::FINISHED:
                std::cout << "[MUX-TEST] All temperatures tested.\n";
                return 0;

            case MuxTestState::ERROR:
                std::cerr << "[MUX-TEST] Aborting test due to error.\n";
                return 1;
        }

        // === TRANSITIONS ===
        switch (currentState) {
            case MuxTestState::INIT:
                currentState = MuxTestState::CONVERT_TEMP;
                break;

            case MuxTestState::CONVERT_TEMP:
                currentState = MuxTestState::SELECT_CHANNEL;
                break;

            case MuxTestState::SELECT_CHANNEL:
                currentState = selectionSuccess ? MuxTestState::WAIT_OSCILLO : MuxTestState::ERROR;
                break;

            case MuxTestState::WAIT_OSCILLO:
                currentState = MuxTestState::NEXT_TEMPERATURE;
                break;

            case MuxTestState::NEXT_TEMPERATURE:
                index++;
                currentState = (index >= temperatures.size()) ? MuxTestState::FINISHED : MuxTestState::CONVERT_TEMP;
                break;

            case MuxTestState::FINISHED:
            case MuxTestState::ERROR:
                // handled by action switch
                break;
        }
    }

    return 0;
}
