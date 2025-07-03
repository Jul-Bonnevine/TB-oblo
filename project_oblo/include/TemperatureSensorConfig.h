#ifndef TEMPERATURE_SENSOR_CONFIG_H
#define TEMPERATURE_SENSOR_CONFIG_H

/**
 * @class TemperatureSensorConfig
 * @brief Stores configuration parameters for a temperature sensor.
 */
class TemperatureSensorConfig {
public:
    /**
     * @brief Constructor with parameters.
     * @param vcc Supply voltage.
     * @param rFixed Fixed resistor value.
     * @param rWires Wire resistance.
     * @param beta Beta value of the thermistor.
     * @param r25 Resistance at 25 degrees Celsius.
     * @param t25 Temperature at 25 degrees Celsius.
     */
    TemperatureSensorConfig(float vcc, float rFixed, float rWires, float beta, float r25, float t25);
   
    /**
     * @brief Get the supply voltage.
     * @return Vcc in volts.
     */
    float getVcc() const{ return Vcc; };

    /**
     * @brief Get the fixed resistor value.
     * @return Fixed resistor value in ohms.
     */
    float getRFixed() const { return R_fixed; };

    /**
     * @brief Get the wire resistance.
     * @return Wire resistance in ohms.
     */
    float getRWires() const { return R_wires; };


    /**
     * @brief Get the Beta coefficient.
     * @return Beta coefficient of the thermistor.
     */
    float getBeta() const { return Beta; };


    /**
     * @brief Get the resistance at 25 °C.
     * @return Resistance in ohms at 25 °C.
     */
    float getR25() const { return R25; };


    /**
     * @brief Get the temperature at 25 °C.
     * @return Temperature in degrees Celsius at 25 °C.
     */
    float getT25() const { return T25; };

private:
    float Vcc;          ///< Supply voltage
    float R_fixed;      ///< Fixed resistor in voltage divider
    float R_wires;      ///< Wire resistance
    float Beta;         ///< Beta coefficient
    float R25;          ///< Resistance at 25°C
    float T25;          ///< Reference temperature (usually 25°C)

};

#endif
