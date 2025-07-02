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

    /*SPI configuration
        - SPI_IOC_WR_MODE: SPI mode
        - SPI_IOC_WR_BITS_PER_WORD: number of bits per frame
        - SPI_IOC_WR_MAX_SPEED_HZ: Maximum transmission speed
    link: https://elixir.bootlin.com/linux/v6.15.4/source/include/uapi/linux/spi/spidev.h#L111
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

    /*Structure to describe an SPI transfer
        - tx_buff : point to the data to be transmitted (MOSI)
        - rx_buff: points to the receive buffer (MISO)
        - len: number of bytes to be transferred
        - speed_hz: set to 0 because speed is used in the constructor
        - delay_usecs: no delay between transfers
        - bit_per_word: set to 0 because the value is used in the constructor
        - cs_change: Chip select change flag (0 = keep CS active, 1 = deactivate after transfer)
    link: https://docs.huihoo.com/doxygen/linux/kernel/3.7/structspi__ioc__transfer.html#ab32597ad72699fd3481059340fdae62c
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
bool SPIInterface::isValid() const 
{
    return fd >= 0;
}