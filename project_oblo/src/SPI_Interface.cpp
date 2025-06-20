#include "SPI_Interface.h"

SPIInterface::SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed)
{
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) 
    {
        perror("open SPI");
        return;
    }

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}
SPIInterface::~SPIInterface()
{
    if (fd >= 0) 
    {
        close(fd);
    }
}
bool SPIInterface::transfer(const uint8_t* tx, uint8_t* rx, size_t len)
{
    if (fd < 0) return false;

    struct spi_ioc_transfer tr 
    {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = static_cast<uint32_t>(len),
        .speed_hz = 0,
        .delay_usecs = 0,
        .bits_per_word = 0,
    };

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

bool SPIInterface::isValid() const 
{
    return fd >= 0;
}