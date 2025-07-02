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

/**
 * @class MainController
 * @brief Central controller class that initializes and provides access to system components.
 */
class MainController {
public:

    /**
     * @brief Initialising objects.
     * Constructs all required components for the system.
     */
    MainController();

    /**
     * @brief Getter for ADC object (used outside the class).
     * @return Reference to ADC instance.
     */
    ADC& getAdc() { return adc; }

    /**
     * @brief Getter for AnalogMultiplexer object (used outside the class).
     * @return Reference to AnalogMultiplexer instance.
     */
    AnalogMultiplexer& getMultiplexer() { return mux; }

    /**
     * @brief Getter for ObloAPI object (used outside the class).
     * @return Reference to ObloAPI instance.
     */
    ObloAPI& getApi() { return api; }

    /**
     * @brief Getter for NTPClient object (used outside the class).
     * @return Reference to NTPClient instance.
     */
    NTPClient& getNtp() { return ntp; }

private:
    SPIInterface adcSpi;    ///< SPI interface for ADC
    SPIInterface muxSpi;    ///< SPI interface for multiplexer
    ADC adc;                ///< ADC object
    AnalogMultiplexer mux;  ///< Analog multiplexer object
    ObloAPI api;            ///< Oblo API communication object
    NTPClient ntp;          ///< NTP client for time synchronization
};

#endif
