#ifndef OBLOAPI_H
#define OBLOAPI_H

#include <string>

class ObloAPI {
public:

    //Initialisation avec l'adresse mac donnée
    explicit ObloAPI(const std::string& mac);

    //Post sur l'api la température mesurée
    bool sendTemperature(float temp);

    //Get température prévuue
    bool getForecast(float& forecast);

    //Get des paramètres pour calculer la température simulée
    bool getParameters(float& n, float& k_m);

private:
    std::string mac_address;
};

#endif // OBLOAPI_H
