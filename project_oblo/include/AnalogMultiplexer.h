#ifndef ANALOGMULTIPLEXER_H
#define ANALOGMULTIPLEXER_H

#include "SPI_Interface.h"
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <cmath>

class AnalogMultiplexer {
public:

    /*Initialise with the interface
        - spi: manage the ADC's SPI communication
    */
    AnalogMultiplexer(SPIInterface& spi);

    //Converts °C to a value between 0-31
    uint8_t convertTemperatureToChannel(float temp);

    //Sending the SPI frame for channel selection
    bool selectChannel(uint8_t channel);

private:
    //Reference to SPI_Interface
    SPIInterface& spi;

};

#endif
