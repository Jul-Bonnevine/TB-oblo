/**
 * @file testNTPClient.cpp
 * @brief Implements a while loop for testing the NTP (Network Time Protocol) client operations.
 */

#include "NTPClient.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() 
{
    /**
     * NTP client initialisation
     * Using Google's public NTP server
     */
    NTPClient client("time.google.com");

    while (true) 
    {
        std::time_t now = client.getCurrentTime();
        if (now == -1) 
        {
            std::cerr << "[NTP] Error retrieving time.\n";
        } else 
        {
            std::cout << "[NTP] Current time: " << std::ctime(&now);
        }
        std::cout << "------------------------------------\n";

        /**
         * Wait for 60 seconds before the next request
         */
        usleep(60000000);
    }
    return 0;
}
