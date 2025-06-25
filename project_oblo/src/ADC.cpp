#include "ADC.h"

ADC::ADC(SPIInterface& spi, float vcc, float r_fixe, float r_fils, float beta, float r25, float t25)
    : spi(spi), Vcc(vcc), R_fixe(r_fixe), R_fils(r_fils), beta(beta), R25(r25), T25(t25)
{

}

bool ADC::sendSetup()
{
    // SETUP register: 0b01100100
    // CKSEL = 10 (clock interne, déclenché par SPI)
    // REFSEL = 01 (référence externe)
    uint8_t tx[1] = { 0x64 };
    uint8_t rx[1] = { 0 };

    return spi.transfer(tx, rx, 1);
}

bool ADC::sendConfig()
{
    if (channel > 3) {
        std::cerr << "[ADC] Canal invalide (MAX11627 a 4 canaux).\n";
        return false;
    }

    // Bit 7 = 1 (conversion register)
    // CHSEL = canal sélectionné
    // SCAN = 11 (one shot, no scan)
    uint8_t tx[1] = {
        static_cast<uint8_t>(0x80 | (channel << 3) | 0x06)
    };
    uint8_t rx[1] = { 0 };

    return spi.transfer(tx, rx, 1);
}

void ADC::setChannel(uint8_t ch)
{
    if (ch < 4) {
        channel = ch;
    } else {
        std::cerr << "[ADC] Canal hors limites (0–3).\n";
    }
}

bool ADC::readRaw(uint16_t& value)
{
    uint8_t tx[2] = { 0x00, 0x00 };
    uint8_t rx[2] = { 0 };

    if (!spi.transfer(tx, rx, 2)) {
        std::cerr << "[ADC] Erreur de transfert SPI lors de la lecture.\n";
        return false;
    }

    /*
        Pour l'ADC. Il faut mettre cette ligne. Il n'y a pas de décalage 
    */
    //value = ((rx[0] & 0x0F) << 8) | rx[1];  // 4 bits de padding + 12 bits MSB-first

    /*
        Pour la génération avec waveform utiliser cette ligne. Un décalage est obligatoire
        à cause d'un mauvais timing
    */
    value = (((rx[0] << 8) | rx[1]) << 1) & 0x0FFF;

    return true;
}


float ADC::readTemperature()
{
    uint16_t adc_val = 0;
    if (!readRaw(adc_val)) {
        std::cerr << "Erreur lecture ADC\n";
        return 0.0f;
    }

    // Étape 1 : conversion valeur ADC → tension
    const float adc_resolution = 4096.0f;
    float Vout = (adc_val * Vcc) / adc_resolution;

    // Étape 2 : calcul résistance sonde
    float Vref = Vcc;
    float Rsonde = ((-1.0f * (Vout * (R_fils + R_fixe) - Vref * R_fils)) /
                   (Vout - Vref));

    // Étape 3 : calcul température avec loi de β
    float tempK = (beta * T25) / (log((Rsonde - R_fils) / R25) * T25 + beta);
    float tempC = tempK - 273.15f;

    return tempC;
}
