/**
 * @file AnalogMultiplexer.cpp
 * @brief Implements the AnalogMultiplexer class for temperature to channel conversion and channel selection via SPI.
 */
#include "AnalogMultiplexer.h"

AnalogMultiplexer::AnalogMultiplexer(SPIInterface& spi) : spi(spi)
{
  
}

uint8_t AnalogMultiplexer::convertTemperatureToChannel(float temp)
{
    //Limit the range from -15°C to 30.5°C
    if (temp < -15.0f) temp = -15.0f;
    if (temp > 31.5f) temp = 31.5f;

    /**
     * Calculation for channel selection as a function of temperature.
     * The formula is derived from the linear mapping of temperature to channel.
     * Rounding is used to ensure the result is an integer.
     */
    int channel = (int)(std::round(-(2.0f / 3.0f) * temp + 21.0f));

    /**
     * The channel is masked with 0x1F to ensure it fits within the 5-bit range (0-31).
     * This is necessary because the multiplexer only supports 32 channels.
     */
    return uint8_t(channel & 0x1F);
}

bool AnalogMultiplexer::selectChannel(uint8_t channel)
{

    /**
     * The channel is masked with 0x1F to ensure it fits within the 5-bit range (0-31).
     * This is necessary because the multiplexer only supports 32 channels.
     * The command is sent as a single byte, where the 5 least significant bits represent the channel.
     * The command format is:
     * - 0b00000CCCCC
     */
    uint8_t command[1] = { uint8_t(channel & 0x1F) };

    /**
     * The analog multiplexer does not have a MISO line,
     * so we do not expect any data back after sending the command.
     */
    uint8_t rx[1] = {0x00};
    /**
    * Sending a single SPI frame (1 byte)
    */
    bool success = spi.transfer(command, rx, 1);
    if (!success) 
    {
    std::cerr << "[MUX] SPI send failed (channel" << (int)(channel) << ")\n";
    return false;
    }

    return true;
}