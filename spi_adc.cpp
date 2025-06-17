#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>

int main() {
    const char* device = "/dev/spidev1.0";  // SPI1, CS0
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Paramètres SPI
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 500000;

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // Trame pour lire canal 0, single-ended, unipolaire
    uint8_t tx[2] = {0x86, 0x00};
    uint8_t rx[2] = {0};

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("ioctl");
    } else {
        // reconstruction valeur 12 bits
        int value = ((rx[0] & 0x0F) << 8) | rx[1];
        std::cout << "Valeur ADC canal 0 : " << value << std::endl;
    }

    close(fd);
    return 0;
}

