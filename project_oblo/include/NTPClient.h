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

    //Initialisation avec time.google.com 
    explicit NTPClient(const std::string& server = "time.google.com");

    //Récupérer l'heure sur le serveur NTP
    std::time_t getCurrentTime();

private:
    std::string ntpServer;
};

#endif // NTPCLIENT_H
