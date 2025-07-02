// testADC.cpp

#include "SPI_Interface.h"
#include "ADC.h"
#include "TemperatureSensorConfig.h"
#include <iostream>
#include <bitset>
#include <unistd.h>

int main() {
    SPIInterface spi("/dev/spidev1.0", SPI_MODE_0, 8, 500000);
    if (!spi.isValid()) {
        std::cerr << "Error: invalid SPI interface.\n";
        return 1;
    }

    // physical parameters of the temperature probe
    TemperatureSensorConfig config(
        3.3f,     // Vcc in volts
        22000.0f, // R_fixed
        3.8f,     // R_wires
        3960.0f,  // Beta
        10000.0f, // R25
        298.15f   // T25 in Kelvin
    );

    ADC adc(spi, config);
    adc.setChannel(0);

        if (!adc.sendSetup()) {
            std::cerr << "Error SETUP\n";
            return 1;
        } 

        usleep(10);

        if (!adc.sendConfig()) {
            std::cerr << "Error CONFIG\n";
            return 1;
        }

        usleep(20);

        uint16_t raw = 0;
        if (!adc.readRaw(raw)) {
            std::cerr << "Error reading raw value\n";
            return 1;
        }

        std::cout << "Raw ADC value: " << std::dec << raw << "\n";

        float temp = adc.readTemperature(raw);
        std::cout << "→ Measured temperature: " << temp << " °C\n";

        return 0;
}
