#ifndef OBLOAPI_H
#define OBLOAPI_H

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

class ObloAPI {
public:

    //Initialisation with the given mac address
    ObloAPI(const std::string& mac);

    //Post the measured temperature to the api
    bool sendTemperature(float temp);

    //Get forecast temperature
    bool getForecast(float& forecast);

    //Get parameters to calculate the simulated temperature
    bool getParameters(float& n, float& k_m);

private:

    //Mac address
    std::string mac_address;

    bool performGet(const std::string& url, std::string& response);
};

#endif
