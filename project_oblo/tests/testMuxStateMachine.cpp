/**
 * @file testMultiplexerStateMachine.cpp
 * @brief Implements a state machine for testing the multiplexer operations.
 */

#include "SPI_Interface.h"
#include "AnalogMultiplexer.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>

/** 
* State machine for the multiplexer test
* Initial state is INIT
* CONVERT_TEMP state is for converting the temperature to the corresponding channel.
* SELECT_CHANNEL state is for selecting the MUX channel.
* WAIT_OSCILLO state is for waiting for the oscilloscope.
* FINISHED state is for indicating that the test is complete.
* ERROR state is for handling errors during the test.
*/
enum class MuxTestState 
{
    INIT,
    CONVERT_TEMP,
    SELECT_CHANNEL,
    WAIT_OSCILLO,
    FINISHED,
    ERROR
};

int main() 
{
    /**
     * Initialisation SPI for Mux
     * The Mux is on SPI0
     * Configuration: Mode 0, 8 bits, 1MHz
     */
    SPIInterface muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000);
    if (!muxSpi.isValid()) 
    {
        std::cerr << "[MUX-TEST] Invalid SPI interface.\n";
        return 1;
    }

    /**
     * Analog Multiplexer initialisation
     * SPI interface
     */
    AnalogMultiplexer mux(muxSpi);

    /**
     * Test temperature to select channel mapping
     */
    float testTemperature = 15.0f;

    /**
     * parameters for the state machine
     */
    float currentTemp;
    uint8_t currentChannel;
    bool selectionSuccess;

    MuxTestState currentState = MuxTestState::INIT;

    while (true) 
    {
        /**
         * Actions to perform in each state
         */
        switch (currentState) 
        {
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
                if (!selectionSuccess) 
                {
                    std::cerr << "[MUX-TEST] SPI failure on channel " << static_cast<int>(currentChannel) << "\n";
                }
                break;

            case MuxTestState::WAIT_OSCILLO:
                std::cout << "[MUX-TEST] Waiting for oscilloscope...\n";
                usleep(500000);
                break;

            case MuxTestState::FINISHED:
                std::cout << "[MUX-TEST] Test finished.\n";
                return 0;

            case MuxTestState::ERROR:
                std::cerr << "[MUX-TEST] Test aborted due to error.\n";
                return 1;
        }

        /**
         * Actions to perform in each state
         */
        switch (currentState) 
        {
            case MuxTestState::INIT:
                currentState = MuxTestState::CONVERT_TEMP;
                break;

            case MuxTestState::CONVERT_TEMP:
                currentState = MuxTestState::SELECT_CHANNEL;
                break;

            case MuxTestState::SELECT_CHANNEL:
                if (selectionSuccess) 
                {
                    currentState = MuxTestState::WAIT_OSCILLO;
                } else 
                {
                    currentState = MuxTestState::ERROR;
                }
                break;


            case MuxTestState::WAIT_OSCILLO:
                currentState = MuxTestState::FINISHED;
                break;

            case MuxTestState::FINISHED:
                break;
            case MuxTestState::ERROR:
                break;
        }
    }

    return 0;
}
