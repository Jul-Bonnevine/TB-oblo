#include "SPI_Interface.h"

SPIInterface::SPIInterface(const std::string& device, uint8_t mode, uint8_t bits, uint32_t speed)
{
    //Ouverture du fichier du périphérique SPI en R/W
    fd = open(device.c_str(), O_RDWR);

    //Gérer les erreurs d'ouverture
    if (fd < 0) 
    {
        perror("open SPI");
        return;
    }

    /*Configuration du SPI
        - SPI_IOC_WR_MODE : mode SPI
        - SPI_IOC_WR_BITS_PER_WORD : Nombre de bits par trame
        - SPI_IOC_WR_MAX_SPEED_HZ : Vitesse maximale de transmission
    link : https://elixir.bootlin.com/linux/v6.15.4/source/include/uapi/linux/spi/spidev.h#L111
    */
    ioctl(fd, SPI_IOC_WR_MODE, &mode);              
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);     
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);     
}
SPIInterface::~SPIInterface()
{
    //Fermeture du fichier du périphérique SPI
    if (fd >= 0) 
    {
        close(fd);
    }
}
bool SPIInterface::transfer(const uint8_t* tx, uint8_t* rx, size_t len)
{
    //En cas d'erreur return false
    if (fd < 0)
    {
        return false;
    }

    /*Structure pour décrire un transfert SPI
        - tx_buff : pointer vers les données à transmettre (MOSI)
        - rx_buff : pointer vers le buffer de réception (MISO)
        - len : nombre de bytes à transférer
        - speed_hz : mis à 0 car on utilise la vitesse dans le constructeur
        - delay_usecs : pas de délai entre les transferts
        - bit_per_word : mis à 0 car on utilise la valeur dans le constructeur
        - cs_change : Chip select change flag (0 = keep CS active, 1 = deactivate after transfer)
    link : https://docs.huihoo.com/doxygen/linux/kernel/3.7/structspi__ioc__transfer.html#ab32597ad72699fd3481059340fdae62c
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

    // Lance le transfert SPI via un ioctl et renvoie le succès
    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}

//Permet de vérifier que le fichier SPI est valide
bool SPIInterface::isValid() const 
{
    return fd >= 0;
}