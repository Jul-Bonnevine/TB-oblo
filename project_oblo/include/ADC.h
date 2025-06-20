#ifndef ADC_H
#define ADC_H

#include "SPI_Interface.h"
#include <cstdint>

class ADC 
{
public:

    //Initialiser avec l'interface
    explicit ADC(SPIInterface& spi);

    //Définition du canal à lire
    void setChannel(uint8_t ch);

    //lecture de la valeur en sortie de l'ADC
    bool readRaw(uint16_t& value);

    //conversion de la valeur de l'ADC en °C (T_mes)
    float readTemperature();

private:
    SPIInterface& spi;
    uint8_t channel = 0;

    //Trame de setup
    bool sendSetup();

    //Trame de configuration
    bool sendConfig();
};

#endif // ADC_H
