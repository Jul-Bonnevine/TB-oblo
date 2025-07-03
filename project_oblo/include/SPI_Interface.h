#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#include <string>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>

/**
 * @class SPIInterface
 * @brief Provides a simple interface to communicate over SPI using Linux spidev.
 */
class SPIInterface 
{
public:
    
    /**
     * @brief Constructor.
     * @param device SPI device file (e.g., "/dev/spidev0.0").
     * @param mode SPI mode (0-3).
     * @param bits Number of bits per frame (usually 8).
     * @param speed Maximum transmission speed in Hz.
     */
    SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed);

    /**
     * @brief Destructor.
     */
    ~SPIInterface();

    /**
     * @brief Reads and writes SPI frames (MOSI/MISO).
     * @param tx Buffer for data to be sent (MOSI).
     * @param rx Buffer for storing received data (MISO).
     * @param len Buffer length (bytes).
     * @return True if the transfer was successful, false otherwise.
     */
    bool transfer(const uint8_t* tx, uint8_t* rx, size_t len);

    /**
     * @brief Checks if the SPI interface is valid.
     * @return True if the SPI interface is valid, false otherwise.
     */
    bool isValid() const;

private:
    int fd; ///< File descriptor for the SPI device.
};

#endif