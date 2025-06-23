#include "AnalogMultiplexer.h"

AnalogMultiplexer::AnalogMultiplexer(SPIInterface& spi) : spi(spi)
{
  
}

uint8_t AnalogMultiplexer::convertTemperatureToChannel(float temp)
{
  // Plage limitée : -15°C à 30.5°C
  if (temp < -15.0f) temp = -15.0f;
  if (temp > 31.5f) temp = 31.5f;

  // Formule : canal = -(2/3) * T + b avec b = 31 (valide pour T = -15)
  int canal = static_cast<int>(std::round(-(2.0f / 3.0f) * temp + 21.0f));
  return static_cast<uint8_t>(canal & 0x1F); // canal codé sur 5 bits
}

bool AnalogMultiplexer::selectChannel(uint8_t channel)
{
  uint8_t command = channel & 0x1F;
  uint8_t rx = 0;

  // Envoi d'une seule trame SPI (1 octet)
  bool success = spi.transfer(&command, &rx, 1);
  if (!success) 
  {
    std::cerr << "[MUX] Échec de l'envoi SPI (canal " << static_cast<int>(channel) << ")\n";
    return false;
  }

  std::cout << "[MUX] Canal " << static_cast<int>(channel)
        << " sélectionné (trame : 0x" << std::hex << static_cast<int>(command) << std::dec << ")\n";
  return true;
}