#include <iostream>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

void fetch_ntp_time() {
    const char* hostname = "time.google.com";
    const int port = 123;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct hostent* server = gethostbyname(hostname);
    if (!server) {
        std::cerr << "Erreur DNS pour " << hostname << std::endl;
        close(sock);
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    unsigned char packet[48] = {0};
    packet[0] = 0b11100011; // LI, Version, Mode

    if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sock);
        return;
    }

    sockaddr_in from{};
    socklen_t from_len = sizeof(from);
    if (recvfrom(sock, packet, sizeof(packet), 0, (sockaddr*)&from, &from_len) < 0) {
        perror("recvfrom");
        close(sock);
        return;
    }

    uint32_t seconds;
    std::memcpy(&seconds, &packet[40], sizeof(seconds));
    seconds = ntohl(seconds) - NTP_TIMESTAMP_DELTA;

    time_t rawtime = seconds;
    std::cout << "Heure NTP : " << ctime(&rawtime);

    close(sock);
}

int main() {
    while (true) {
        fetch_ntp_time();
        sleep(60); // attendre 60 secondes
    }
    return 0;
}
