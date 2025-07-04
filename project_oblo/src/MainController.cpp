/**
 * @file MainController.cpp
 * @brief Implements the MainController class which sets up all system components.
 */
#include "MainController.h"

MainController::MainController() :adcSpi("/dev/spidev1.0", SPI_MODE_0, 8, 500000),
    muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000),
    adc(adcSpi, TemperatureSensorConfig(
          3.3f,    // Vcc
          22000.0f, // R_fixed
          3.8f,    // R_wires
          3960.0f, // Beta
          10000.0f, // R25
          298.15f  // T25
      )),
      mux(muxSpi),
      api("0030DEABCDEF"),
      ntp("time.google.com")
{
    
}
