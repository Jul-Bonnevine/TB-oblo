#include "MainController.h"
#include "Simulator.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <cmath>

// Définition des états
enum class TestState {
    READ_ADC,
    POST_TEMP,
    FETCH_API_DATA,
    COMPUTE_SIMULATION,
    SELECT_MUX_CHANNEL,
    SYNC_TIME,
    WAIT_NEXT_CYCLE
};

int main() {
    // Initialisation
    MainController controller;

    if (!(controller.getAdc().getSpi().isValid() && controller.getMultiplexer().getSpi().isValid())) {
        std::cerr << "[MAIN] SPI initialization failed.\n";
        return 1;
    }

    // Variables d’état
    TestState currentState = TestState::READ_ADC;
    float T_mes = NAN, T_pred = NAN, T_sim = NAN;
    float n = 0.0f, k_m = 0.0f;
    std::time_t now = 0;
    int loop_test = 1;
    int channel_tested = 0;

    // Boucle principale
    while (true) {
        std::cout << "\n===== [Test #" << loop_test << "] Channel " << channel_tested << " =====\n";

        // === ACTIONS SELON L’ÉTAT COURANT ===
        switch (currentState) {
            case TestState::READ_ADC:
                // Lire et convertir température
                break;

            case TestState::POST_TEMP:
                // Envoyer température à l'API
                break;

            case TestState::FETCH_API_DATA:
                // Récupérer prévisions + paramètres
                break;

            case TestState::COMPUTE_SIMULATION:
                // Calculer T_sim
                break;

            case TestState::SELECT_MUX_CHANNEL:
                // Convertir T_sim en canal MUX
                break;

            case TestState::SYNC_TIME:
                // Récupérer l'heure NTP
                break;

            case TestState::WAIT_NEXT_CYCLE:
                // Attente + changer de canal + incrémenter test
                break;
        }

        // === TRANSITIONS VERS L’ÉTAT SUIVANT ===
        switch (currentState) {
            case TestState::READ_ADC:
                // Si T_mes valide -> POST_TEMP, sinon -> WAIT_NEXT_CYCLE
                break;

            case TestState::POST_TEMP:
                currentState = TestState::FETCH_API_DATA;
                break;

            case TestState::FETCH_API_DATA:
                currentState = TestState::COMPUTE_SIMULATION;
                break;

            case TestState::COMPUTE_SIMULATION:
                currentState = TestState::SELECT_MUX_CHANNEL;
                break;

            case TestState::SELECT_MUX_CHANNEL:
                currentState = TestState::SYNC_TIME;
                break;

            case TestState::SYNC_TIME:
                currentState = TestState::WAIT_NEXT_CYCLE;
                break;

            case TestState::WAIT_NEXT_CYCLE:
                currentState = TestState::READ_ADC;
                break;
        }
    }

    return 0;
}
