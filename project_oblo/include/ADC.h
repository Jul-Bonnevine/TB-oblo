#ifndef ADC_H
#define ADC_H

#include "SPI_Interface.h"
#include "TemperatureSensorConfig.h"
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <cmath>

class ADC 
{
public:

    /*Initialiser avec l'interface et les paramètres de la sonde
        - spi : gérer la communication SPI de l'ADC
        - congif : configurer la sonde de température mesurée
    */
    ADC(SPIInterface& spi, const TemperatureSensorConfig& config);
    
    //Trame de setup envoyée à l'ADC
    bool sendSetup();

    //Trame de configuration
    bool sendConfig();

    //Définition du canal à lire
    void setChannel(uint8_t ch);

    //lecture de la valeur en sortie de l'ADC
    bool readRaw(uint16_t& value);

    //conversion de la valeur de l'ADC en °C (T_mes)
    float readTemperature(uint16_t adc_val);

private:

    //Référence vers SPI_Interface
    SPIInterface& spi;

    //Initialisation du cannal, sur le canal 0
    uint8_t channel = 0;

    //Paramètres réel de la sonde
    TemperatureSensorConfig config;
};

#endif
