/**
 * @file ADC.cpp
 * @brief Implements the ADC class for temperature acquisition via SPI and conversion in °C.
 */
#include "ADC.h"

ADC::ADC(SPIInterface& spi, const TemperatureSensorConfig& config)
    : spi(spi), config(config)
{

}

bool ADC::sendSetup()
{
    /**
     * Send setup frame.
     * CKSEL1 : 1, CKSEL : 1 to set the external clock.
     * REFSEL1 : 0, REFSEL0 : 1 to set the external reference voltage.
     * 0b011101xx sent to MAX11627.
    */
    uint8_t tx_setup[1] = { 0x74 };

    /**
     * Rx[1] is not used, but needed for transfer.
     * The MAX11627 does not send any data back.
     * When the setup frame is sent.
    */
    uint8_t rx[1] = { 0x00 };
    return spi.transfer(tx_setup, rx, 1);
}

bool ADC::sendConfig()
{
    if (channel > 3) 
    {
        std::cerr << "[ADC] Invalid channel (MAX11627 has 4 channels).\n";
        return false;
    }

    /**
     * Send frame
     * Bit 7 to 1 to activate conversion.
     * CHSEL3:CHSEL0 to 0 to read channel 0 for example.
     * SCAN1:SCAN0 to 1 not to scan channels, just read channel 0.
     */
    uint8_t tx_config[1] = { (uint8_t)(0x80 | (channel << 3) | 0x06) };

    /**
     * Rx[1] is not used, but needed for transfer.
     * The MAX11627 does not send any data back.
     * When the config frame is sent.
     */
    uint8_t rx[1] = { 0x00 };

    return spi.transfer(tx_config, rx, 1);
}

void ADC::setChannel(uint8_t ch)
{
    if (ch < 4) 
    {
        channel = ch;
    } else {
        std::cerr << "[ADC] Channel out of bounds (0-3).\n";
    }
}

bool ADC::readRaw(uint16_t& value)
{

    /**
     * We just send a empty frame to the ADC
     * to read the current value.
     * Tx[2] is not used, but needed for transfer.
    */
    uint8_t tx[2] = {0x00, 0x00};

    /**
     * We need to read 2 bytes (1 byte for the 4 most significant bits
     * and 1 byte for the 8 least significant bits).
    */
    uint8_t rx_value_raw[2] = {0x00, 0x00};

    if (!spi.transfer(tx, rx_value_raw, 2)) 
    {
        std::cerr << "[ADC] SPI transfer error during reading.\n";
        return false;
    }

    /**
     * Rx[0] contains the 4 most significant bits of the ADC value [xxxx D11 D10 D9 D8].
     * Rx[1] contains the 8 least significant bits of the ADC value [D7 D6 D5 D4 D3 D2 D1 D0].
     * Rx[0] & 0x0F: mask bits not useful for 1st byte.
     * << 8 : set this 1st byte to the most significant bit.
     * | Rx[1] adds 8 LSB bits.
    */
    value = ((rx_value_raw[0] & 0x0F) << 8) | rx_value_raw[1];
    return true;
}

float ADC::readTemperature(uint16_t adc_val)
{
    const float adc_resolution = 4096.0f;

    //Get all probe parameters
    float Vcc = config.getVcc();
    float R_fixed = config.getRFixed();
    float R_wires = config.getRWires();
    float beta = config.getBeta();
    float R25 = config.getR25();
    float T25 = config.getT25();

    
    // divider bridge output voltage calculation based on ADC value
    float Vout = (adc_val * Vcc) / adc_resolution;
    float Vref = Vcc;


    //Calculating the resistive value of the probe   
    float Rprobe = ((-1.0f * (Vout * (R_wires + R_fixed) - Vref * R_wires)) / (Vout - Vref));
    float Rntc = Rprobe - R_wires;


    //Checking if the calculated values are valid
    std::cout << "ADC value: " << adc_val << "\n";
    std::cout << "Vout: " << Vout << " V\n";
    std::cout << "Measured probe: " << Rprobe << " ohms\n";
    std::cout << "Measured probe without wires: " << Rntc << " ohms\n";
    

    if (Rntc <= 0.0f) {
        std::cerr << "[ADC] Invalid NTC resistor : " << Rntc << " ohms\n";
        return NAN; // Return NaN if the resistance is invalid
    }

    // Calculation of sensor temperature
    float tempK = (beta * T25) / (log(Rntc / R25) * T25 + beta);
    float tempC = tempK - 273.15f;
    return tempC;
}
