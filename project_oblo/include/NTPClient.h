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

class NTPClient {
public:

    //Initialising the server with time.google.com
    NTPClient(const std::string& server = "time.google.com");

    //Retrieve the time from the NTP server
    std::time_t getCurrentTime();

private:
    std::string ntpServer;
};

#endif
