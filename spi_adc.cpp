#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>

const char* SPI_DEVICE = "/dev/spidev1.0";
const uint32_t SPI_SPEED = 500000;
const uint8_t SPI_MODE = SPI_MODE_0;
const uint8_t SPI_BITS = 8;

int init_spi(const char* device) {
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    ioctl(fd, SPI_IOC_WR_MODE, &SPI_MODE);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &SPI_BITS);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &SPI_SPEED);

    return fd;
}

bool send_setup(int fd) {
    uint8_t tx[1] = { 0x90 };  // 1001 0000 : setup, clk interne, ref externe, unipolaire
    uint8_t rx[1] = { 0 };

    spi_ioc_transfer tr {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 1,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SETUP failed");
        return false;
    }

    return true;
}

int main() {
    int fd = init_spi(SPI_DEVICE);
    if (fd < 0) return 1;

    std::cout << "Envoi de la trame SETUP toutes les 500 ms...\n";
    std::cout << "Appuie sur Ctrl+C pour arrêter.\n";

    while (true) {
        if (!send_setup(fd)) break;
        std::cout << "Trame SETUP envoyée (0x90)\n";
        usleep(500000); // 500 ms
    }

    close(fd);
    return 0;
}
