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

    //Constructeur
    MainController();

    //Getter des objets pour un accès hors classe
    ADC& getAdc() { return adc; }
    AnalogMultiplexer& getMultiplexer() { return mux; }
    ObloAPI& getApi() { return api; }
    NTPClient& getNtp() { return ntp; }

private:

    //Déclaration des objets
    SPIInterface adcSpi;
    SPIInterface muxSpi;
    ADC adc;
    AnalogMultiplexer mux;
    ObloAPI api;
    NTPClient ntp;
};

#endif
