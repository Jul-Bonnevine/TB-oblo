#include "ObloAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;
/*
    link : https://github.com/voidlib/cpp-curl-example/blob/main/2_http_get_post.cpp
*/

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
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << temp;
    std::string tempStr = oss.str();

    std::string requestUrl =
        "https://dev.oblosolutions.ch/tb25hesso_param?mac_address=" + mac_address +
        "&measured_temp=" + tempStr;

    CURL* curlHandle = curl_easy_init();
    if (!curlHandle) return false;

    CURLcode result;
    std::string responseBuffer;

    curl_easy_setopt(curlHandle, CURLOPT_URL, requestUrl.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseBuffer);
    curl_easy_setopt(curlHandle, CURLOPT_POST, 1L);

    result = curl_easy_perform(curlHandle);
    if (result != CURLE_OK) {
        std::cerr << "POST failed: " << curl_easy_strerror(result) << std::endl;
        curl_easy_cleanup(curlHandle);
        return false;
    }

    std::cout << "POST request URL: " << requestUrl << std::endl;
    std::cout << "Server response: " << responseBuffer << std::endl;

    curl_easy_cleanup(curlHandle);
    return true;
}

bool ObloAPI::getForecast(float& forecast) {
    std::string requestUrl =
        "https://dev.oblosolutions.ch/tb25hesso_forecast?mac_address=" + mac_address;
    std::string responseBuffer;

    if (!performGet(requestUrl, responseBuffer)) return false;

    try {
        json data = json::parse(responseBuffer);
        if (data.contains("forecast") &&
            !data["forecast"].empty() &&
            data["forecast"][0].contains("temperature")) {
            forecast = data["forecast"][0]["temperature"];
            return true;
        }

        std::cerr << "Forecast JSON invalid." << std::endl;
        return false;
    } catch (const json::exception& e) {
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
    if (!curlHandle) return false;

    CURLcode result;

    curl_easy_setopt(curlHandle, CURLOPT_URL, requestUrl.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseBuffer);
    curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 0L);

    result = curl_easy_perform(curlHandle);
    if (result != CURLE_OK) {
        std::cerr << "GET failed: " << curl_easy_strerror(result) << std::endl;
        curl_easy_cleanup(curlHandle);
        return false;
    }

    std::cout << "GET request URL: " << requestUrl << std::endl;
    std::cout << "Server response: " << responseBuffer << std::endl;

    curl_easy_cleanup(curlHandle);
    return true;
}
