#ifndef SIMULATOR_H
#define SIMULATOR_H

/**
 * @class Simulator
 * @brief Provides static method to compute simulated temperature.
 */
class Simulator {
public:

    /**
     * @brief Computes the simulated temperature based on measured temperature, forecast temperature,
     *        and parameters n and k_m.
     * @param T_mes Measured temperature in degrees Celsius.
     * @param T_forecast Forecast temperature in degrees Celsius.
     * @param n Parameter n for the simulation.
     * @param k_m Parameter k_m for the simulation.
     * @return Simulated temperature in degrees Celsius.
     */
    static float computeSimulatedTemperature(float T_mes, float T_forecast, float n, float k_m);
};

#endif
