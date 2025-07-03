/**
 * @file NTPClient.cpp
 * @brief Implements the NTPClient class for retrieving time from an NTP server via UDP.
 */
#include "NTPClient.h"

#define NTP_TIMESTAMP_DELTA 2208988800ull   //seconds since 1 January 1900

NTPClient::NTPClient(const std::string& server) 
{
    ntpServer = server;
}

 std::time_t NTPClient::getCurrentTime()
 {
    /**
     * Creation of the UDP socket.
     * AF_INET : IPV4 socket.
     * SOCK_DGRAM: UDP mode.
     * IPPROTO_UDP: protocol used.
     */
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
         std::cerr << "Error creation socket\n";
        return -1;
    } 

    /**
     * 2 second timeout on reception.
     * Avoids indefinite blockages.
    */
    struct timeval timeout = {2, 0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    /**
     * Structure for getaddrinfo().
     * AF_INET : filter for IPv4.
     * SOCK_DGRAM : filter for UDP.
     * IPPROTO : UDP protocol.
    */
    struct addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;


    /**
     * Resolving time.google.com as an IP address. The UDP socket needs
     * a binary IP address to be able to send data over the network.
     */
    if (getaddrinfo(ntpServer.c_str(), "123", &hints, &res) != 0 || !res) {
        std::cerr << "[NTP] Failed to resolve DNS\n";
        close(sock);
        return -1;
    }

    /**
     * Creation of the 48 byte array.
     * Standard size of an NTP packet (RFC 5905).
     * Contains the request sent and the response from the server.
     */
    unsigned char packet[48] = {0};

    /**
     * NTP packet structure
     * LI (Leap Indicator) = 3 : clock non synchro
     * VN (Version Number) = 4 : version du protole NTP
     * Mode = 3 : client
     * 0b11100011
     */
    packet[0] = 0b11100011;

    /**
     * Sends the NTP packet to the IP address and NTP port of the server.
     * res->ai_addr : contains the IP of time.google.com.
     * res->ai_addrlen : size of this address.
     */
    if (sendto(sock, packet, sizeof(packet), 0, res->ai_addr, res->ai_addrlen) < 0) {
        std::cerr << "[NTP] Failed to send\n";
        freeaddrinfo(res);
        close(sock);
        return -1;
    }

    /**
     * Wait for a response from the server. Packet stored in packet.
     * The last two params are nullptr because the sender's address is
     * not important.
     */
    if (recvfrom(sock, packet, sizeof(packet), 0, nullptr, nullptr) < 0) {
        std::cerr << "[NTP] Failed to receive (timeout ?)\n";
        freeaddrinfo(res);
        close(sock);
        return -1;
    }

    /**
     * Releasing allocated memory
     * closing the UDP socket
     */
    freeaddrinfo(res);
    close(sock);

    /**
     * The NTP timestamp sent by the
     * server is in the 4 bytes at offset 40.
     * memcpy(): copies the 4 bytes of packet[40]
     * into seconds.
     */
    uint32_t seconds = 0;
    std::memcpy(&seconds, &packet[40], sizeof(seconds));

    // Conversion big endian in little endian
    seconds = ntohl(seconds); 

    /**
     * Conversion NTP  Unix.
     * NTP counts from 01.01.1900.
     * Time_t (UNIX) counts from 01.01.1970.
     * It is therefore necessary to make this subtraction to have the correct year.
     */
    return (std::time_t)(seconds - NTP_TIMESTAMP_DELTA);
 }