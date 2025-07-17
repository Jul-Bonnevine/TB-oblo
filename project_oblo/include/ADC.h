#ifndef ADC_H
#define ADC_H

#include "SPI_Interface.h"
#include "TemperatureSensorConfig.h"
#include <cstdint>
#include <unistd.h>
#include <iostream>
#include <cmath>

/**
 * @class ADC
 * @brief Configures the ADC and retrieves its values. Manages the conversion of raw values to temperature.
 */
class ADC 
{
public:
    /**
     * @brief Constructor.
     * @param spi Reference to the SPI interface used to communicate with the ADC.
     * @param config Configuration parameters for the connected temperature probe.
    */
    ADC(SPIInterface& spi, const TemperatureSensorConfig& config);
    
    /**
     * @brief Sends the setup frame to the ADC.
     * @return True if the SPI transmission was successful.
     */
    bool sendSetup();

    /**
     * @brief Sends the configuration frame to the ADC.
     * @return True if the SPI transmission was successful.
     */
    bool sendConfig();

    /**
     * @brief Selects the input channel to be read.
     * @param ch Channel number initialized to 0.
     */
    void setChannel(uint8_t ch);

    /**
     * @brief Reads the raw digital output from the ADC.
     * @param value Reference to store the ADC raw value (12 bits).
     * @return True if the read was successful.
     */
    bool readRaw(uint16_t& value);

    /**
     * @brief Converts a raw ADC value to temperature in °C.
     * @param adc_val Raw ADC value.
     * @return Temperature measured in degrees Celsius.
     */
    float readTemperature(uint16_t adc_val);

    /**
     * @brief Gets the SPI interface used by the ADC. Inline function for easy access.
     * @return Reference to the SPIInterface instance.
     */
    SPIInterface& getSpi() { return spi; }


private:
    SPIInterface& spi;               ///< Reference to SPI interface.
    uint8_t channel = 0;            ///< Currently selected input channel.
    TemperatureSensorConfig config; ///< Parameters of the connected temperature probe.
};

#endif
