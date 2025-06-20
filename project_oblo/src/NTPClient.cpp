#include "NTPClient.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull

NTPClient::NTPClient(const std::string& server) {
    ntpServer = server;
}

 std::time_t NTPClient::getCurrentTime()
 {
    return 0;
 }