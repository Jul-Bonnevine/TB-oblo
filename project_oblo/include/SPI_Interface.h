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
    /*Initialostion de l'interface SPI
        - device : chemin vers le périphérique
        - mode : mode du SPI
        - bits : nombre de bits par trame
        - speed : vitesse de la communication SPI
    */
    SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed);
    ~SPIInterface();

    /*Lecture et écriture des trames SPI (MOSI/MISO)
        - tx : buffer des données à envoyer
        - rx : buffer pour stocker les données reçues
        - len : longueur des buffers (bytes)
    */
    bool transfer(const uint8_t* tx, uint8_t* rx, size_t len);

    //Vérification que SPI soit actif sur le raspberry
    bool isValid() const;

private:
    //file descriptor du SPI
    int fd;
};

#endif