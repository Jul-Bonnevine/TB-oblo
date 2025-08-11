#include "SPI_Interface.h"
#include "AnalogMultiplexer.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>

// États de la machine
enum class MuxTestState {
    INIT,
    CONVERT_TEMP,
    SELECT_CHANNEL,
    WAIT_OSCILLO,
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

    // Température de test configurable
    float testTemperature = 15.0f;

    // Variables de test (initialisées dans INIT)
    float currentTemp;
    uint8_t currentChannel;
    bool selectionSuccess;

    MuxTestState currentState = MuxTestState::INIT;

    while (true) {
        // === ACTIONS ===
        switch (currentState) {
            case MuxTestState::INIT:
                std::cout << "[MUX-TEST] Initialization OK\n";
                currentTemp = 0.0f;
                currentChannel = 0;
                selectionSuccess = true;
                break;

            case MuxTestState::CONVERT_TEMP:
                currentTemp = testTemperature;
                currentChannel = mux.convertTemperatureToChannel(currentTemp);
                std::cout << "[MUX-TEST] T = " << currentTemp << " °C --> channel " << static_cast<int>(currentChannel) << "\n";
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

            case MuxTestState::FINISHED:
                std::cout << "[MUX-TEST] Test finished.\n";
                return 0;

            case MuxTestState::ERROR:
                std::cerr << "[MUX-TEST] Test aborted due to error.\n";
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
                if (selectionSuccess) {
                    currentState = MuxTestState::WAIT_OSCILLO;
                } else {
                    currentState = MuxTestState::ERROR;
                }
                break;


            case MuxTestState::WAIT_OSCILLO:
                currentState = MuxTestState::FINISHED;
                break;

            case MuxTestState::FINISHED:
            case MuxTestState::ERROR:
                // handled in action switch
                break;
        }
    }

    return 0;
}
