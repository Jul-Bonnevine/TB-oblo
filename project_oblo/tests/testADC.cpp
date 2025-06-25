// testADC.cpp

#include "SPI_Interface.h"
#include "ADC.h"
#include <iostream>
#include <bitset>
#include <unistd.h>

int main() {
    SPIInterface spi("/dev/spidev1.0", SPI_MODE_0, 8, 500000);
    if (!spi.isValid()) {
        std::cerr << "Erreur : interface SPI non valide.\n";
        return 1;
    }

    ADC adc(spi,
            3.3f,    // Vcc en volts
            22000.0f, // R_fixe
            3.8f,    // R_fils
            3960.0f, // Bêta
            10000.0f, // R25
            298.15f  // T25 (Kelvin)
    );

    adc.setChannel(0);

    while (true) {
        if (!adc.sendSetup()) {
            std::cerr << "Erreur SETUP\n";
            break;
        }

        usleep(10);

        if (!adc.sendConfig()) {
            std::cerr << "Erreur CONFIG\n";
            break;
        }

        usleep(20);

        uint16_t raw = 0;
        if (!adc.readRaw(raw)) {
            std::cerr << "Erreur lecture raw\n";
            break;
        }

        std::cout << "Trame brute corrigée : 0x" << std::hex << raw
                  << " (0b" << std::bitset<12>(raw) << ")\n";

        float temp = adc.readTemperature();
        std::cout << "→ Température mesurée : " << std::dec << temp << " °C\n";

        std::cout << "------------------------------\n";
        usleep(500000);  // 500 ms
    }

    return 0;
}
