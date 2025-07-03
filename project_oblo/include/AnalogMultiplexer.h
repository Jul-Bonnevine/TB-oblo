#ifndef ANALOGMULTIPLEXER_H
#define ANALOGMULTIPLEXER_H

#include "SPI_Interface.h"
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <cmath>

/**
 * @class AnalogMultiplexer
 * @brief Converts the temperature into a channel for the analogue multiplexer.
 *        Manages channel selection via SPI.
 */
class AnalogMultiplexer {
public:

    /**
     * @brief Constructor.
     * @param spi Reference to the SPI interface used to control the multiplexer.
     */
    AnalogMultiplexer(SPIInterface& spi);

    /**
     * @brief Converts a temperature (°C) into a 5-bit channel index (0-31).
     * @param temp Temperature in degrees Celsius.
     * @return Corresponding channel index (0 to 31).
     */
    uint8_t convertTemperatureToChannel(float temp);

    /**
     * @brief Sends the SPI frame to select the desired analog channel.
     * @param channel Channel number to select (0 to 31).
     * @return True if the SPI transmission was successful.
     */
    bool selectChannel(uint8_t channel);

    /**
     * @brief Gets the SPI interface used by the multiplexer.
     * @return Reference to the SPIInterface instance.
     */
    SPIInterface& getSpi() { return spi; }


private:
    SPIInterface& spi; ///< Reference to the SPI interface used for communication.

};

#endif
