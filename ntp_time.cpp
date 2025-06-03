#include <iostream>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

int main() {
    const char* hostname = "time.google.com";
    const int port = 123;

    // Créer le socket UDP
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    // Résolution DNS
    struct hostent* server = gethostbyname(hostname);
    if (!server) {
        std::cerr << "Erreur DNS pour " << hostname << std::endl;
        return 1;
    }

    // Configuration de l'adresse serveur
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    std::memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Préparer le paquet NTP
    unsigned char packet[48] = {0};
    packet[0] = 0b11100011; // LI, Version, Mode

    // Envoyer la requête
    if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        return 1;
    }

    // Recevoir la réponse
    sockaddr_in from{};
    socklen_t from_len = sizeof(from);
    if (recvfrom(sock, packet, sizeof(packet), 0, (sockaddr*)&from, &from_len) < 0) {
        perror("recvfrom");
        return 1;
    }

    // Extraire l'heure
    uint32_t seconds;
    std::memcpy(&seconds, &packet[40], sizeof(seconds));
    seconds = ntohl(seconds) - NTP_TIMESTAMP_DELTA;

    time_t rawtime = seconds;
    std::cout << "Heure NTP : " << ctime(&rawtime);

    close(sock);
    return 0;
}
