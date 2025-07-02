#ifndef OBLOAPI_H
#define OBLOAPI_H

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

/**
 * @class ObloAPI
 * @brief Handles communication with the Oblo API (GET and POST).
 */
class ObloAPI {
public:

    /**
     * @brief Constructor.
     * @param mac MAC address of the device.
     */
    ObloAPI(const std::string& mac);

    /**
     * @brief Posts the measured temperature to the API.
     * @param temp Temperature to send.
     * @return True if the operation was successful.
     */
    bool sendTemperature(float temp);

    /**
     * @brief Gets the forecast temperature.
     * @param forecast Reference to store the forecast temperature.
     * @return True if the operation was successful.
     */
    bool getForecast(float& forecast);

    /**
     * @brief Gets parameters to calculate the simulated temperature.
     * @param n Reference to store parameter n.
     * @param k_m Reference to store parameter k_m.
     * @return True if the operation was successful.
     */
    bool getParameters(float& n, float& k_m);

private:


    std::string mac_address; ///< MAC address of the device.

    /**
     * @brief Performs a GET request.
     * @param url URL to send the request to.
     * @param response Reference to store the response.
     * @return True if the operation was successful.
     */
    bool performGet(const std::string& url, std::string& response);
};

#endif
