#include "SPI_Interface.h"

SPIInterface::SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed)
{

}
SPIInterface::~SPIInterface()
{

}
bool SPIInterface::transfer(const uint8_t* tx, uint8_t* rx, size_t len)
{
    return true;
}

bool SPIInterface::isValid() const 
{
    return true;
}