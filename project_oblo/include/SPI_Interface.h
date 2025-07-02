#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#include <string>
#include <cstdint>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>

class SPIInterface 
{
public:
    /*SPI interface initialization
        - device: path to the device
        - mode: SPI mode
        - bits: number of bits per frame
        - speed: SPI communication speed
    */
    SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed);
    ~SPIInterface();

    /*Read and write SPI frames (MOSI/MISO)
        - tx: buffer for data to be sent
        - rx: buffer for storing received data
        - len: buffer length (bytes)
    */
    bool transfer(const uint8_t* tx, uint8_t* rx, size_t len);

    //Check that the SPI is active on the raspberry
    bool isValid() const;

private:
    //SPI file descriptor
    int fd;
};

#endif