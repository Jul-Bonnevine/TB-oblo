#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <string>
#include <ctime>

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
