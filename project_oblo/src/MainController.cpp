#include "MainController.h"

MainController::MainController() :adcSpi("/dev/spidev1.0", SPI_MODE_0, 8, 500000),
    muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000),
    adc(adcSpi, TemperatureSensorConfig(
          3.3f,    // Vcc
          2000.0f, // R_fixe
          3.8f,    // R_fils
          3950.0f, // Beta
          1000.0f, // R25
          298.15f  // T25
      )),
      mux(muxSpi),
      api("0030DEABCDEF"),
      ntp("time.google.com")
{
    
}
