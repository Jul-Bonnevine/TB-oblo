#ifndef ANALOGMULTIPLEXER_H
#define ANALOGMULTIPLEXER_H

#include "SPI_Interface.h"
#include <cstdint>
#include <iostream>
#include <unistd.h>
#include <cmath>

class AnalogMultiplexer {
public:

    // Initalisation avec le SPI
    explicit AnalogMultiplexer(SPIInterface& spi);

    //Conversion °C -> valeur entre 0-31
    uint8_t convertTemperatureToChannel(float temp);

    //Envoie de la trame SPI pour la sélection du canal
    bool selectChannel(uint8_t channel);

private:
    SPIInterface& spi;

};

#endif // ANALOGMULTIPLEXER_H
