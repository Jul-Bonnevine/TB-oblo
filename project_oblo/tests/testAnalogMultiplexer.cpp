#include "SPI_Interface.h"
#include "AnalogMultiplexer.h"
#include <iostream>
#include <unistd.h>
#include <cstdint>

int main() {
    // Température unique à tester (modifiable à la main)
    float testTemperature = 17.5f;

    // Initialisation SPI
    SPIInterface muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000);
    if (!muxSpi.isValid()) {
        std::cerr << "[MUX-TEST] Invalid SPI interface.\n";
        return 1;
    }

    // Création de l’objet multiplexer
    AnalogMultiplexer mux(muxSpi);

    // Conversion température → canal
    uint8_t channel = mux.convertTemperatureToChannel(testTemperature);
    std::cout << "[MUX-TEST] T = " << testTemperature << " °C → channel " << static_cast<int>(channel) << "\n";

    // Sélection du canal via SPI
    if (!mux.selectChannel(channel)) {
        std::cerr << "[MUX-TEST] SPI failure for channel " << static_cast<int>(channel) << "\n";
        return 1;
    }

    // Pause pour observation oscilloscope
    usleep(500000); // 500 ms

    std::cout << "[MUX-TEST] Test completed.\n";
    return 0;
}
