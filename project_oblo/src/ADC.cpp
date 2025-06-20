#include "ADC.h"

bool ADC::sendSetup()
{
 return false;
}

bool ADC::sendConfig()
{
    return false;
}

ADC::ADC(SPIInterface& spi) : spi(spi)
{

}

void ADC::setChannel(uint8_t ch)
{

}

bool ADC::readRaw(uint16_t& value)
{
    return false;
}


float ADC::readTemperature()
{
    return 0;
}
