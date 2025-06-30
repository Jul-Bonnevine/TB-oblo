#include "ADC.h"

ADC::ADC(SPIInterface& spi, const TemperatureSensorConfig& config)
    : spi(spi), config(config)
{

}

bool ADC::sendSetup()
{
    /*Envoie de la trame
        - CKSEL1 : 1, CKSEL : 1 pour fixer le clock en externe
        - REFSEL1 : 0, REFSEL0 : 1 pour fixer la tension de référence en externe

        ==> 0b011101xx envoyé sur le MAX11627
    */
    uint8_t tx[1] = { 0x74 };
    uint8_t rx[1] = { 0x00 };
    return spi.transfer(tx, rx, 1);
}

bool ADC::sendConfig()
{
    if (channel > 3) 
    {
        std::cerr << "[ADC] Canal invalide (MAX11627 a 4 canaux).\n";
        return false;
    }

    /*Envoie de la trame
        - bit 7 à 1 pour activer la conversion
        - CHSEL3:CHSEL0 à 0 pour lire le canal 0 par exemple
        - SCAN1:SCAN0 à 1 pour ne pas scanner les canaux, simple lecture du canal 0
    */
    uint8_t tx[1] = { (uint8_t)(0x80 | (channel << 3) | 0x06) };
    uint8_t rx[1] = { 0x00 };

    return spi.transfer(tx, rx, 1);
}

void ADC::setChannel(uint8_t ch)
{
    if (ch < 4) 
    {
        channel = ch;
    } else 
        {
            std::cerr << "[ADC] Canal hors limites (0–3).\n";
        }
}

bool ADC::readRaw(uint16_t& value)
{

    uint8_t tx[2] = {0x00, 0x00};

    /*
        -Attente des 2 bytes pour qui contiennent la valeurs de l'ADC
    */
    uint8_t rx[2] = {0x00, 0x00};

    if (!spi.transfer(tx, rx, 2)) 
    {
        std::cerr << "[ADC] Erreur de transfert SPI lors de la lecture.\n";
        return false;
    }

    /*
        -rx[0] : [xxxx D11 D10 D9 D8] 4 bits de poids fort
        -rx[1] : [D7 D6 D5 D4 D3 D2 D1 D0] 8 bits de poids faible
        -rx[0] & 0x0F : masquer les bits non utile pour de 1er byte
        - << 8 : placer ce 1er byte en poid fort
        - | rx[1] ajoute des 8 bits de LSB
    */
    value = ((rx[0] & 0x0F) << 8) | rx[1];
    return true;
}

float ADC::readTemperature(uint16_t adc_val)
{
    const float adc_resolution = 4096.0f;

    float Vcc = config.getVcc();
    float R_fixe = config.getRFixe();
    float R_fils = config.getRFils();
    float beta = config.getBeta();
    float R25 = config.getR25();
    float T25 = config.getT25();

    /*
        Caclul de la tension de sortie du pont diviseur 
        en se basant sur la valeur de l'ADC
    */
    float Vout = (adc_val * Vcc) / adc_resolution;
    float Vref = Vcc;

    /*
        Calcul de la valeur de résistive de la sonde   
    */
    float Rsonde = ((-1.0f * (Vout * (R_fils + R_fixe) - Vref * R_fils)) / (Vout - Vref));
    float Rntc = Rsonde - R_fils;

    /*
        Print des valeurs intermédiaires
    */
    
    std::cout << "ADC brut: " << adc_val << "\n";
    std::cout << "Vout: " << Vout << " V\n";
    std::cout << "Rsonde: " << Rsonde << " ohms\n";
    std::cout << "Rsonde - R_fils: " << Rntc << " ohms\n";
    

    if (Rntc <= 0.0f) {
        std::cerr << "[ADC] Résistance NTC invalide : " << Rntc << " ohms\n";
        return NAN;
    }

    /*
        Calcul de la température de la sonde
    */
    float tempK = (beta * T25) / (log(Rntc / R25) * T25 + beta);
    float tempC = tempK - 273.15f;
    return tempC;
}
