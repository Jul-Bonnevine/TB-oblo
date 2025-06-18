#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cmath>

using json = nlohmann::json;

// Callback pour curl
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// [1] Envoi température mesurée (POST)
bool sendTemperature(const std::string& mac_address, float temp) {
    std::string url = "https://dev.oblosolutions.ch/tb25hesso_param";
    url += "?mac_address=" + mac_address + "&measured_temp=" + std::to_string(temp);

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res == CURLE_OK;
}

// [2] Récupération météo prévue (GET)
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
    } catch (json::exception& e) {
        std::cerr << "Erreur JSON (forecast) : " << e.what() << std::endl;
        return false;
    }
}

// [3] Récupération des paramètres (GET)
bool getParam(const std::string& mac_address, float& n_out, float& k_m_out) {
    std::string url = "https://dev.oblosolutions.ch/tb25hesso_param?mac_address=" + mac_address;
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
        n_out = data["param"][0]["n"];
        k_m_out = data["param"][0]["k_m"];
        return true;
    } catch (json::exception& e) {
        std::cerr << "Erreur JSON (paramètres) : " << e.what() << std::endl;
        return false;
    }
}

float calculateSimulatedTemperature(float T_mes, float T_prevu, float n, float k_m) {
    if (T_prevu > T_mes) {
        float delta = T_prevu - T_mes;
        float T_sim = T_mes + delta * n * std::exp(-k_m);
        return T_sim;
    } else {
        return T_mes; // pas de simulation nécessaire
    }
}

// ==== MAIN ====

int main() {
    std::string mac = "0030DEABCDEF";
    float measured_temp;

    std::cout << "Entrez la température mesurée : ";
    std::cin >> measured_temp;

    if (!sendTemperature(mac, measured_temp)) {
        std::cerr << "Échec lors de l'envoi de la température.\n";
        return 1;
    }

    std::cout << "Température envoyée avec succès.\n";

    float forecast_temp = 0;
    if (!getForecast(mac, forecast_temp)) {
        std::cerr << "Impossible de récupérer la météo.\n";
        return 1;
    }

    std::cout << "Température météo prévue : " << forecast_temp << "°C\n";

    float n = 0, k_m = 0;
    if (!getParam(mac, n, k_m)) {
        std::cerr << "Impossible de récupérer les paramètres.\n";
        return 1;
    }

    std::cout << "Paramètres récupérés : n = " << n << ", k_m = " << k_m << "\n";

    if (forecast_temp > measured_temp) {
        float T_sim = calculateSimulatedTemperature(measured_temp, forecast_temp, n, k_m);
        std::cout << "Température simulée : " << T_sim << "°C\n";
    } else {
        std::cout << "Pas de simulation nécessaire (T_prevu <= T_mesurée).\n";
    }

    return 0;
}

