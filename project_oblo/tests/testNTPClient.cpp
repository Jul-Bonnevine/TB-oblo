#include "NTPClient.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    NTPClient client("time.google.com");

    while (true) {
        std::time_t now = client.getCurrentTime();

        if (now == -1) {
            std::cerr << "[NTP] Erreur lors de la récupération de l'heure.\n";
        } else {
            std::cout << "[NTP] Heure actuelle : " << std::ctime(&now);
        }

        std::cout << "------------------------------------\n";
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}
