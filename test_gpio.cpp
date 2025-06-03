#include <gpiod.h>
#include <iostream>
#include <unistd.h>

int main() {
    const char* chipname = "gpiochip0";
    const unsigned int line_num = 17; // GPIO 17
    const char* consumer = "cm5_output";

    // Ouvre le contrôleur GPIO
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        std::cerr << "Erreur : impossible d'ouvrir " << chipname << std::endl;
        return 1;
    }

    // Récupère la ligne GPIO 17
    gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    if (!line) {
        std::cerr << "Erreur : ligne GPIO non trouvée" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Configure la ligne en sortie
    if (gpiod_line_request_output(line, consumer, 0) < 0) {
        std::cerr << "Erreur : impossible de demander la ligne en sortie" << std::endl;
        gpiod_chip_close(chip);
        return 1;
    }

    // Alterne l'état de la broche
    for (int i = 0; i < 10; ++i) {
        gpiod_line_set_value(line, i % 2); // ON, OFF, ON...
        std::cout << "GPIO " << line_num << " = " << (i % 2) << std::endl;
        sleep(1);
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
