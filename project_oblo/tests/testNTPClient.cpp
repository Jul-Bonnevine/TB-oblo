#include "NTPClient.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() 
{
    NTPClient client("time.google.com");

    while (true) 
    {
        std::time_t now = client.getCurrentTime();

        if (now == -1) {
            std::cerr << "[NTP] Error retrieving time.\n";
        } else {
            std::cout << "[NTP] Current time: " << std::ctime(&now);
        }

        std::cout << "------------------------------------\n";
        
        usleep(60000000); // Wait for 60 seconds before the next request

    }

    return 0;
}
