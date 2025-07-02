#include "AnalogMultiplexer.h"

AnalogMultiplexer::AnalogMultiplexer(SPIInterface& spi) : spi(spi)
{
  
}

uint8_t AnalogMultiplexer::convertTemperatureToChannel(float temp)
{
    //Limit the range from -15°C to 30.5°C
    if (temp < -15.0f) temp = -15.0f;
    if (temp > 31.5f) temp = 31.5f;

    /*
        Calculation for channel selection as a function of temperature
    */
    int canal = (int)(std::round(-(2.0f / 3.0f) * temp + 21.0f));

    /*
        Only the 5 low-weight bits are returned in an uint8_t. These 5 bits are used 
        to activate one of the 32 channels.
    */
    return uint8_t(canal & 0x1F);
}

bool AnalogMultiplexer::selectChannel(uint8_t channel)
{
  
  uint8_t command[1] = { uint8_t(channel & 0x1F) };
  uint8_t rx[1] = {0x00};

  // Sending a single SPI frame (1 byte)
  bool success = spi.transfer(command, rx, 1);
  if (!success) 
  {
    std::cerr << "[MUX] SPI send failed (channel" << (int)(channel) << ")\n";
    return false;
  }

  std::cout << "[MUX] Channel " << (int)(channel)
        << " selected (frame : 0x" << std::hex << (int)(command[0]) << std::dec << ")\n";
  return true;
}