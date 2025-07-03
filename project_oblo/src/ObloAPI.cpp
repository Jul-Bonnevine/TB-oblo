/**
 * @file ObloAPI.cpp
 * @brief Implements the ObloAPI class for communicating with the Oblo web service.
 * 
 * Based on: https://github.com/voidlib/cpp-curl-example/blob/main/2_http_get_post.cpp
 */
#include "ObloAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

/**
 * Callback function for writing received data to a string.
 * @param receivedData Buffer containing received data.
 * @param dataSize Size of each data block received.
 * @param dataBlocks Number of data blocks received.
 * @param outputBuffer Pointer to a string where the received data will be stored.
 * @return The total number of bytes taken care of. If this number differs from 
 * the number of bytes provided, it'll signal an error to the library.
 * 
 * The prototype for this callback function is described here:
 * https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 * @note This function is taken from the example in the link above.
 */
static size_t WriteCallback(
    char* receivedData,
    size_t dataSize,
    size_t dataBlocks,
    void* outputBuffer
) {
    std::string* strBuffer = static_cast<std::string*>(outputBuffer);
    strBuffer->append(receivedData, dataSize * dataBlocks);
    return dataSize * dataBlocks;
}

ObloAPI::ObloAPI(const std::string& mac) : mac_address(mac)
{

}

bool ObloAPI::sendTemperature(float temp)
{
    /**
     * Send the temperature data to the API.
     * The temperature is formatted to two decimal places
     * using std::ostringstream and std::setprecision.
     */
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << temp;
    std::string tempStr = oss.str();

    std::string requestUrl =
        "https://dev.oblosolutions.ch/tb25hesso_param?mac_address=" + mac_address +
        "&measured_temp=" + tempStr;

    CURL* curlHandle = curl_easy_init();
    if (curlHandle)
    {
        CURLcode result;
        std::string responseBuffer;

        // Set the target URL for the POST request
        curl_easy_setopt(curlHandle, CURLOPT_URL, requestUrl.c_str());

        // Specify the POST body (empty in this case, data is sent via URL parameters)
        curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, "");

        // Set the callback function to handle the server's response
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Provide a pointer to the response buffer where data will be stored
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseBuffer);

        result = curl_easy_perform(curlHandle);
        if (result != CURLE_OK) 
        {
            std::cerr << "POST failed: " << curl_easy_strerror(result) << std::endl;
            return false;
        }

        std::cout << "POST request URL: " << requestUrl << std::endl;
        std::cout << "Server response: " << responseBuffer << std::endl;

        curl_easy_cleanup(curlHandle);
        return true;
    }
    else
    {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }
}

bool ObloAPI::getForecast(float& forecast)
{
    // Construct the request URL with the MAC address
    std::string requestUrl =
        "https://dev.oblosolutions.ch/tb25hesso_forecast?mac_address=" + mac_address;
    std::string responseBuffer;

    // Perform the GET request to retrieve the forecast
    performGet(requestUrl, responseBuffer);

    try 
    {

        // Parse the JSON response
        json data = json::parse(responseBuffer);
        /**
        * Check if the JSON contains the "forecast" key and if it has a valid structure.
        * The forecast temperature is expected to be in the first element of the "forecast" array.
        * If the structure is valid, extract the temperature value.
        */
        /*if (data.contains("forecast") &&
            !data["forecast"].empty() &&
            data["forecast"][0].contains("temperature")) 
            {*/
            forecast = data["forecast"][0]["temperature"];
            return true;
            //}

        std::cerr << "Forecast JSON invalid." << std::endl;
        return false;
    } 
    // Handle JSON parsing errors
    catch (const json::exception& e) 
    {
        std::cerr << "JSON error: " << e.what() << std::endl;
        return false;
    }
}

bool ObloAPI::getParameters(float& n, float& k_m) {
    std::string requestUrl =
        "https://dev.oblosolutions.ch/tb25hesso_param?mac_address=" + mac_address;
    std::string responseBuffer;

    if (!performGet(requestUrl, responseBuffer)) return false;

    try {
        json data = json::parse(responseBuffer);
        if (data.contains("param") && !data["param"].empty()) {
            const auto& param = data["param"][0];
            if (param.contains("n") && param.contains("k_m")) {
                n = param["n"];
                k_m = param["k_m"];
                return true;
            }
        }

        std::cerr << "Parameter JSON invalid." << std::endl;
        return false;
    } catch (const json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        return false;
    }
}

bool ObloAPI::performGet(const std::string& requestUrl, std::string& responseBuffer) {
    CURL* curlHandle = curl_easy_init();
    if (curlHandle)
    {
        CURLcode result;

        // Set the target URL for the GET request
        curl_easy_setopt(curlHandle, CURLOPT_URL, requestUrl.c_str());

        // Set the callback function to handle the server's response
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Provide a pointer to the response buffer where data will be stored
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseBuffer);

        // Explicitly set GET mode (optional if no POST/PUT is set)
        curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);

        result = curl_easy_perform(curlHandle);
        if (result != CURLE_OK) 
        {
            std::cerr << "GET failed: " << curl_easy_strerror(result) << std::endl;
            return false;
        }

        std::cout << "GET request URL: " << requestUrl << std::endl;
        std::cout << "Server response: " << responseBuffer << std::endl;

        curl_easy_cleanup(curlHandle);
        return true;
    }
    else
    {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return false;
    }

    
}
