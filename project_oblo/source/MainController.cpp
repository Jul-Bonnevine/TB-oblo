#include "MainController.h"
#include <iostream>
#include <unistd.h>
#include <ctime>

MainController::MainController()
    : adcSpi("/dev/spidev1.0", SPI_MODE_0, 8, 500000),
      muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000),
      adc(adcSpi),
      mux(muxSpi),
      api("0030DEABCDEF"),
      ntp("time.google.com") {}
{

}

void MainController::run()
{

}

void MainController::processOneCycle(uint8_t adc_channel)
{
    
}