#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <string>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>

/**
 * @class NTPClient
 * @brief Retrieves the current time from an NTP server using UDP.
 */
class NTPClient {
public:

    /**
     * @brief Constructor.
     * @param server NTP server address (default: time.google.com).
     */
    NTPClient(const std::string& server = "time.google.com");

    /**
     * @brief Retrieves the current time from the NTP server.
     * @return Current time as a std::time_t value.
     */
    std::time_t getCurrentTime();

private:
    std::string ntpServer; ///< NTP server address.
};

#endif
