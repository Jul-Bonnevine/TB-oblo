#include "SPI_Interface.h"
#include "AnalogMultiplexer.h"
#include <iostream>
#include <unistd.h>
#include <vector>

int main() {
    SPIInterface muxSpi("/dev/spidev0.0", SPI_MODE_0, 8, 1000000);
    if (!muxSpi.isValid()) {
        std::cerr << "[MUX-TEST] Invalid SPI interface.\n";
        return 1;
    }

    AnalogMultiplexer mux(muxSpi);

    // Temperatures to test: cover the entire useful range
    std::vector<float> temperatures = {
        -20.0f, -15.0f, -10.0f, 0.0f, 7.5f, 15.0f, 17.5f, 25.0f, 30.0f, 31.5f
    };

    for (float temp : temperatures) {
        uint8_t canal = mux.convertTemperatureToChannel(temp);
        std::cout << "[MUX-TEST] T = " << temp << " °C to channel " << static_cast<int>(canal) << "\n";

        if (!mux.selectChannel(canal)) {
            std::cerr << "[MUX-TEST] SPI failure for channel " << static_cast<int>(canal) << "\n";
        }

        usleep(500000);  // for oscilloscope observation
    }

    return 0;
}
