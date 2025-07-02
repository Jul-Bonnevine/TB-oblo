#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator {
public:

    //Calculation of simulated temperature
    static float computeSimulatedTemperature(float T_mes, float T_forecast, float n, float k_m);
};

#endif
