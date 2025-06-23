#include "ADC.h"

ADC::ADC(SPIInterface& spi, float vcc, float r_fixe, float r_fils, float beta, float r25, float t25)
    : spi(spi), Vcc(vcc), R_fixe(r_fixe), R_fils(r_fils), beta(beta), R25(r25), T25(t25)
{

}

bool ADC::sendSetup()
{
 return false;
}

bool ADC::sendConfig()
{
    return false;
}

void ADC::setChannel(uint8_t ch)
{

}

bool ADC::readRaw(uint16_t& value)
{
    return false;
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
