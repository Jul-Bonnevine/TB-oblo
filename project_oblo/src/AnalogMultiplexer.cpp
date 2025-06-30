#include "AnalogMultiplexer.h"

AnalogMultiplexer::AnalogMultiplexer(SPIInterface& spi) : spi(spi)
{
  
}

uint8_t AnalogMultiplexer::convertTemperatureToChannel(float temp)
{
    // Limiter la palge de -15°C à 30.5°C
    if (temp < -15.0f) temp = -15.0f;
    if (temp > 31.5f) temp = 31.5f;

    /*
        Calcul pour la sélection de canal en fonction de la température
    */
    int canal = (int)(std::round(-(2.0f / 3.0f) * temp + 21.0f));

    /*
        On retourne seulement les 5bits de poid faible dans un uint8_t. Ces 5 bits permettent 
        l'activation d'un des 32 canaux
    */
    return uint8_t(canal & 0x1F);
}

bool AnalogMultiplexer::selectChannel(uint8_t channel)
{
  
  uint8_t command[1] = { uint8_t(channel & 0x1F) };
  uint8_t rx[1] = {0x00};

  // Envoi d'une seule trame SPI (1 octet)
  bool success = spi.transfer(command, rx, 1);
  if (!success) 
  {
    std::cerr << "[MUX] Échec de l'envoi SPI (canal " << (int)(channel) << ")\n";
    return false;
  }

  std::cout << "[MUX] Canal " << (int)(channel)
        << " sélectionné (trame : 0x" << std::hex << (int)(command[0]) << std::dec << ")\n";
  return true;
}