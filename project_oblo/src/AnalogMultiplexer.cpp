#include "AnalogMultiplexer.h"

AnalogMultiplexer::AnalogMultiplexer(SPIInterface& spi) : spi(spi)
{

}

uint8_t AnalogMultiplexer::convertTemperatureToChannel(float temp)
{
  return 0;
}

bool AnalogMultiplexer::selectChannel(uint8_t channel)
{
    return false;
}