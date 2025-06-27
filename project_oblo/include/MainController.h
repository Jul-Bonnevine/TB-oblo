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
    MainController();

    // Accesseurs pour les composants
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
};

#endif // MAINCONTROLLER_H
