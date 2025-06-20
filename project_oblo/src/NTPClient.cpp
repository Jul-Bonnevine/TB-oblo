#include "NTPClient.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull

NTPClient::NTPClient(const std::string& server) {
    ntpServer = server;
}

 std::time_t NTPClient::getCurrentTime()
 {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) return -1;

    struct hostent* server = gethostbyname(ntpServer.c_str());
    if (!server) {
        close(sock);
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(123);
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    unsigned char packet[48] = { 0 };
    packet[0] = 0b11100011;

    sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&server_addr, sizeof(server_addr));

    sockaddr_in from{};
    socklen_t from_len = sizeof(from);
    recvfrom(sock, packet, sizeof(packet), 0, (sockaddr*)&from, &from_len);

    uint32_t seconds;
    std::memcpy(&seconds, &packet[40], sizeof(seconds));
    seconds = ntohl(seconds) - NTP_TIMESTAMP_DELTA;

    close(sock);
    return static_cast<std::time_t>(seconds);
 }