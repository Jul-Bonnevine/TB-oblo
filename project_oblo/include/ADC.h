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

    /*Initialise with interface and probe parameters
        - spi: manage the ADC's SPI communication
        - congif: configure the measured temperature sensor
    */
    ADC(SPIInterface& spi, const TemperatureSensorConfig& config);
    
    //Setup frame sent to ADC
    bool sendSetup();

    //Configuration frame
    bool sendConfig();

    //Definition of the channel to be read
    void setChannel(uint8_t ch);

    //reading the ADC output value
    bool readRaw(uint16_t& value);

    //conversion of ADC value to °C
    float readTemperature(uint16_t adc_val);

private:

    //Reference to SPI_Interface
    SPIInterface& spi;

    //Channel initialisation
    uint8_t channel = 0;

    //Actual probe parameters
    TemperatureSensorConfig config;
};

#endif
