#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Callback pour curl
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Envoi température mesurée (POST)
bool sendTemperature(const std::string& mac_address, float temp) {
    std::string url = "https://dev.oblosolutions.ch/tb25hesso_param";
    url += "?mac_address=" + mac_address + "&measured_temp=" + std::to_string(temp);

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");  // corps vide
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

// Récupération météo prévue (GET)
bool getForecast(const std::string& mac_address, float& temperature_out) {
    std::string url = "https://dev.oblosolutions.ch/tb25hesso_forecast?mac_address=" + mac_address;
    std::string response;

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) return false;

    try {
        json data = json::parse(response);
        temperature_out = data["forecast"][0]["temperature"];
        return true;
    } catch (...) {
        return false;
    }
}

int main() {
    std::string mac = "0030DEABCDEF";
    float measured_temp;

    std::cout << "Entrez la température mesurée : ";
    std::cin >> measured_temp;

    if (sendTemperature(mac, measured_temp)) {
        std::cout << "Température envoyée avec succès.\n";
    } else {
        std::cerr << "Échec lors de l'envoi de la température.\n";
    }

    float forecast_temp = 0;
    if (getForecast(mac, forecast_temp)) {
        std::cout << "Température météo prévue : " << forecast_temp << "°C\n";
    } else {
        std::cerr << "Impossible de récupérer la météo.\n";
    }

    return 0;
}
