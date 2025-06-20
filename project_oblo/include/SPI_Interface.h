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
    //Constructeur : utilisé pour initialiser l'interface SPI
    SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed);
    ~SPIInterface();

    //Lecture et écriture des trames SPI (MOSI/MISO)
    bool transfer(const uint8_t* tx, uint8_t* rx, size_t len);

    //Vérification que SPI soit actif sur le raspberry
    bool isValid() const;

private:
    
    int fd;
};

#endif // SPIINTERFACE_H