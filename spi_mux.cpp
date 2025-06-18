#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <cstdlib>

// Configuration SPI
bool configureSPI(int fd, uint8_t mode, uint8_t bits, uint32_t speed) {
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) return false;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) return false;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) return false;
    return true;
}

// Construction de la trame ADG731
uint8_t buildMUXCommand(uint8_t channel) {
    return channel & 0x1F; // EN = 0, CS = 0, X = 0, A4:A0 = canal
}

// Envoi d'une trame SPI (1 octet)
bool sendSPIByte(int fd, uint8_t command, uint32_t speed, uint8_t bits) {
    uint8_t tx[1] = { command };
    uint8_t rx[1] = { 0 };

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 1,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

int main() {
    const char* device = "/dev/spidev0.0";
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 1000000; // 1 MHz

    // Ouverture du périphérique SPI
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Configuration du SPI
    if (!configureSPI(fd, mode, bits, speed)) {
        std::cerr << "Erreur de configuration SPI.\n";
        close(fd);
        return 1;
    }

    // Lecture du canal
    int mux_channel;
    std::cout << "Entrez un numéro de canal (0 à 31) : ";
    std::cin >> mux_channel;

    if (std::cin.fail() || mux_channel < 0 || mux_channel > 31) {
        std::cerr << "Entrée invalide. Veuillez entrer un entier entre 0 et 31.\n";
        close(fd);
        return 1;
    }

    uint8_t command = buildMUXCommand(static_cast<uint8_t>(mux_channel));

    // Boucle d'envoi
    while (true) {
        if (sendSPIByte(fd, command, speed, bits)) {
            std::cout << "Canal " << mux_channel << " envoyé (trame : 0x"
                      << std::hex << (int)command << ")." << std::dec << std::endl;
        } else {
            perror("SPI send error");
        }

        usleep(500000); // 500 ms
    }

    close(fd);
    return 0;
}
