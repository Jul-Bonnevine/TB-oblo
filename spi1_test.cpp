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

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 12;
    uint32_t speed = 500000;

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    uint8_t tx[] = {0xAA, 0x55};  // motif test
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
        std::cout << "SPI1 fonctionne !\n";
        std::cout << "Données RX (inutilisables sans périphérique) : "
                  << std::hex << (int)rx[0] << " " << (int)rx[1] << std::endl;
    }

    close(fd);
    return 0;
}
