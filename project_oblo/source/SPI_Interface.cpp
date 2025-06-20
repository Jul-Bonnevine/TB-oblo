#include "SPIInterface.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>

SPIInterface::SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed)
{
    fd = open(device, O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}
SPIInterface::~SPIInterface()
{

}
bool SPIInterface::transfer(const uint8_t* tx, uint8_t* rx, size_t len)
{
    return true;
}

bool SPIInterface::isValid() const 
{
    return true;
}