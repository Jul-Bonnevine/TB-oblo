#include <iostream>
#include <bitset>
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
    uint8_t tx[2] = { 0x00, 0x00 };

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

        usleep(10);

        if (!send_config(fd)) {
            std::cerr << "Erreur CONFIG\n";
            break;
        }

        usleep(20);

        uint8_t rx[2] = { 0 };
        if (!read_adc_raw(fd, rx)) {
            std::cerr << "Erreur lecture\n";
            break;
        }

        // Trame brute
        uint16_t raw = (rx[0] << 8) | rx[1];
        std::cout << "Trame brute : 0x" << std::hex << raw
                  << " (0b" << std::bitset<16>(raw) << ")\n";

        // Correction d’un décalage d’un bit vers la gauche
        uint16_t corrected = ((raw << 1) & 0x0FFF);

        std::cout << "→ Valeur corrigée  : " << std::dec << corrected
                  << " / 4095 (0b" << std::bitset<12>(corrected) << ")\n";

        std::cout << "------------------------------\n";

        usleep(500000);
    }

    close(fd);
    return 0;
}

/*
Version for ADC

    #include <iostream>
#include <bitset>
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
    uint8_t tx[2] = { 0x00, 0x00 };

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

        usleep(10);

        if (!send_config(fd)) {
            std::cerr << "Erreur CONFIG\n";
            break;
        }

        usleep(20);

        uint8_t rx[2] = { 0 };
        if (!read_adc_raw(fd, rx)) {
            std::cerr << "Erreur lecture\n";
            break;
        }

        // Affichage brut des octets reçus
        std::cout << "RX[0]: 0x" << std::hex << (int)rx[0]
                  << " (" << std::bitset<8>(rx[0]) << "), RX[1]: 0x"
                  << std::hex << (int)rx[1]
                  << " (" << std::bitset<8>(rx[1]) << ")\n";

        uint16_t value = ((rx[0] & 0x0F) << 8) | rx[1];
        std::cout << "Valeur ADC : " << std::dec << value << " / 4095 (0b"
                  << std::bitset<12>(value) << ")\n";

        usleep(500000);
    }

    close(fd);
    return 0;
}

*/