#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "SPI_Interface.h"
#include "ADC.h"
#include "AnalogMultiplexer.h"
#include "ObloAPI.h"
#include "Simulator.h"
#include "NTPClient.h"
#include <iostream>
#include <unistd.h>
#include <ctime>

class MainController {
public:

    //Initialisation des modules
    MainController();

    //programme de test
    //void run();

    //getter déclaré en inLine
    ADC& getAdc() { return adc; }
    AnalogMultiplexer& getMultiplexer() { return mux; }
    ObloAPI& getApi() { return api; }
    NTPClient& getNtp() { return ntp; }

private:
    SPIInterface adcSpi;
    SPIInterface muxSpi;
    ADC adc;
    AnalogMultiplexer mux;
    ObloAPI api;
    NTPClient ntp;

    //Processus de test complet
    //void processOneCycle(uint8_t adc_channel);
};

#endif // MAINCONTROLLER_H
