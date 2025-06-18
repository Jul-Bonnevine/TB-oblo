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
    // b01010100 → setup register, clock mode 10 (CKSEL=10), ref externe (REFSEL=01)
    uint8_t tx[1] = { 0x54 };
    uint8_t rx[1] = { 0 };

    spi_ioc_transfer tr {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 1,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS,
    };

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

bool send_config(int fd) {
    // b10000000 → conversion register, CHSEL=0000 (AIN0), SCAN=00 (canal seul)
    uint8_t tx[1] = { 0x80 };
    uint8_t rx[1] = { 0 };

    spi_ioc_transfer tr {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 1,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS,
    };

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

bool read_adc_raw(int fd, uint8_t* rx) {
    uint8_t tx[2] = { 0x00, 0x00 }; // envoie 16 bits nuls, lit 16 bits depuis DOUT

    spi_ioc_transfer tr {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = SPI_SPEED,
        .bits_per_word = SPI_BITS,
    };

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

int main() {
    int fd = init_spi(SPI_DEVICE);
    if (fd < 0) return 1;

    std::cout << "Setup → Config → Lecture (affiche sur oscilloscope SCLK + MISO)\n";

    while (true) {
        if (!send_setup(fd)) {
            std::cerr << "Erreur SETUP\n";
            break;
        }

        usleep(10); // délai entre SETUP et CONFIG

        if (!send_config(fd)) {
            std::cerr << "Erreur CONFIG\n";
            break;
        }

        usleep(10); // attendre la fin de conversion (~2 µs)

        uint8_t rx[2] = { 0 };
        if (!read_adc_raw(fd, rx)) {
            std::cerr << "Erreur lecture\n";
            break;
        }

        uint16_t value = ((rx[0] & 0x0F) << 8) | rx[1];
        std::cout << "Valeur ADC : " << value << " / 4095\n";

        usleep(500000); // 500 ms
    }

    close(fd);
    return 0;
}
