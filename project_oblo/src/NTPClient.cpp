#include "NTPClient.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>

NTPClient::NTPClient(const std::string& server = "time.google.com")
{

}

 std::time_t NTPClient::getCurrentTime()
 {
    
 }