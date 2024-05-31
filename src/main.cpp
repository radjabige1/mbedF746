#include <mbed.h>
#include <threadLvgl.h>
#include <InterruptIn.h>
#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);

// Définition des objets pour la LED et le bouton
DigitalOut led1(LED1);
DigitalIn button(PI_11);
DigitalOut buzzer(D10);

bool button_was_pressed = false;


// Boucle principale
int main() {
    // Configure le bouton en mode PullUp (le bouton est actif bas)
    button.mode(PullUp);

    while (true) {

        while (true) {
        // Vérifie si le bouton est pressé (état bas)
            if (button == 0) {
            // Si le bouton était auparavant relâché, il vient d'être pressé
                if (!button_was_pressed) {
                // Inverse l'état de la LED
                led1 = !led1;

                if (led1)
                {
                    buzzer = 1;
                }
                else{
                    buzzer = 0;
                }

                // Marque le bouton comme pressé
                button_was_pressed = true;
                }
            }

            // Vérifie si le bouton est relâché (état haut)
            if (button == 1) {
                // Réinitialise l'état du bouton
                button_was_pressed = false;
            }

            // Attente pour décharger le processeur (50 ms)
            ThisThread::sleep_for(50ms);
        }
    }
}