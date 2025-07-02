#include "ObloAPI.h"
using json = nlohmann::json;

// Fonction callback pour CURL, qui écrit la réponse dans une string
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

ObloAPI::ObloAPI(const std::string& mac)
{
    mac_address = mac;
}

bool ObloAPI::sendTemperature(float temp)
{
    std::string url = "https://dev.oblosolutions.ch/tb25hesso_param?mac_address=" + mac_address +
                      "&measured_temp=" + std::to_string(temp);

    std::cout << "Envoi vers : " << url << std::endl;

    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);      //Ligne pour voir les logs en cas d'erreurs

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return res == CURLE_OK;
}

bool ObloAPI::getForecast(float& forecast)
{
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
        forecast = data["forecast"][0]["temperature"];
        return true;
    } catch (json::exception& e) {
        std::cerr << "Error JSON (forecast) : " << e.what() << std::endl;
        return false;
    }
}

bool ObloAPI::getParameters(float& n, float& k_m)
{
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
        n = data["param"][0]["n"];
        k_m = data["param"][0]["k_m"];
        return true;
    } catch (json::exception& e) {
        std::cerr << "Error JSON (parameters) : " << e.what() << std::endl;
        return false;
    }
}
