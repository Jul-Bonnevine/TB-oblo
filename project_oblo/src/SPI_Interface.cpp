/**
 * @file SPI_Interface.cpp
 * @brief Implements the SPIInterface class to handle SPI communication on Linux using spidev.
 */
#include "SPI_Interface.h"

SPIInterface::SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed)
{
    //Open SPI device file in R/W
    fd = open(device.c_str(), O_RDWR);

    //Manage opening errors
    if (fd < 0) 
    {
        perror("open SPI");
        return;
    }

    /*
     * Configure SPI interface
     * SPI_IOC_WR_MODE: Sets the SPI mode
     * SPI_IOC_WR_BITS_PER_WORD: Sets the number of bits per word
     * SPI_IOC_WR_MAX_SPEED_HZ: Sets the max speed (Hz)
     * 
     * Ref: https://elixir.bootlin.com/linux/v6.15.4/source/include/uapi/linux/spi/spidev.h#L111
     */
    ioctl(fd, SPI_IOC_WR_MODE, &mode);              
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);     
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);     
}
SPIInterface::~SPIInterface()
{
    //Closing the SPI device file
    if (fd >= 0) 
    {
        close(fd);
    }
}
bool SPIInterface::transfer(const uint8_t* tx, uint8_t* rx, size_t len)
{
    //In the event of an error return false
    if (fd < 0)
    {
        return false;
    }

    /*
     * Prepare SPI transfer struct:
     * tx_buf: Pointer to data sent on MOSI
     * rx_buf: Pointer to buffer for data received on MISO
     * len: Number of bytes transferred
     * speed_hz, bits_per_word: 0 = use defaults set earlier
     * delay_usecs: 0 = no delay
     * cs_change: 0 = keep chip select active after transfer
     *
     * Ref: https://docs.huihoo.com/doxygen/linux/kernel/3.7/structspi__ioc__transfer.html
     */
    struct spi_ioc_transfer tr 
    {
        .tx_buf = (uint64_t)tx,
        .rx_buf = (uint64_t)rx,
        .len = (uint32_t)(len),
        .speed_hz = 0,
        .delay_usecs = 0,
        .bits_per_word = 0,
        .cs_change = 0,
    };

    // Starts SPI transfer via ioctl and returns success
    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

// Checks that the SPI file is valid
bool SPIInterface::isValid()
{
    return fd >= 0;
}