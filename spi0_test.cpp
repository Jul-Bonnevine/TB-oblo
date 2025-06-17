#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cstring>

void test_spi(const char* device) {
    std::cout << "=== Test sur " << device << " ===" << std::endl;

    int fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 500000;

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    uint8_t tx[] = {0xAA, 0x55};  // Motif test
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
        std::cout << "Transfert réussi. RX = "
                  << std::hex << (int)rx[0] << " " << (int)rx[1] << std::dec << "\n";
    }

    close(fd);
    std::cout << std::endl;
}

int main() {
    test_spi("/dev/spidev0.0"); // SPI0, CS0
    test_spi("/dev/spidev0.1"); // SPI0, CS1
    test_spi("/dev/spidev0.2"); // SPI0, CS2
    test_spi("/dev/spidev0.3"); // SPI0, CS3
    return 0;
}
