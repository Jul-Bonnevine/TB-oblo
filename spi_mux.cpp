#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>
#include <cstdlib>

int main() {
    const char* device = "/dev/spidev0.0"; // SPI0, CS0
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 1000000; // 1 MHz

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    int mux_channel;
    std::cout << "Entrez un numéro de canal (0 à 31) : ";
    std::cin >> mux_channel;

    if (std::cin.fail() || mux_channel < 0 || mux_channel > 31) {
        std::cerr << "Entrée invalide. Veuillez entrer un entier entre 0 et 31.\n";
        close(fd);
        return 1;
    }

    // Trame SPI : 16 bits, seuls les bits A4:A0 utiles
    uint16_t command = static_cast<uint16_t>(mux_channel);
    uint8_t tx[2] = {
        static_cast<uint8_t>(command >> 8),   // MSB
        static_cast<uint8_t>(command & 0xFF)  // LSB
    };

    uint8_t rx[2] = {0};

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    std::cout << "Envoi répété toutes les 500 ms...\nAppuie sur Ctrl+C pour arrêter.\n";

    while (true) {
        if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
            perror("ioctl");
        } else {
            std::cout << "Canal " << mux_channel << " envoyé via SPI.\n";
        }

        usleep(500000); // pause 500 ms = 0.5 s
    }

    close(fd);
    return 0;
}
