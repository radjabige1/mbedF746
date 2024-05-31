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

I2C my_i2c(D14,D15);
const int SRF08_ADDRESS = 0xE0;

bool button_was_pressed = false;


// Boucle principale
int main() {

    char init_tab[2] = {0x00, 0x51};
    char data[2];
    char reg_data_dist[1]={0x02}; // Registre de données de distance
    int distance;

    // Configure le bouton en mode PullUp (le bouton est actif bas)
    button.mode(PullUp);

    my_i2c.frequency(100000);

    buzzer = 0;

    while (true) {
        my_i2c.write(SRF08_ADDRESS, init_tab, 2);

        ThisThread::sleep_for(100ms);

        my_i2c.write(SRF08_ADDRESS, reg_data_dist, 1);
        my_i2c.read(SRF08_ADDRESS, data, 2);
        distance = (data[0] << 8) + data[1];
        printf("Distance: %d cm\n", distance);
        // Vérifie si le bouton est pressé (état bas)
        // if (button == 0) 
        // {
        //     // Si le bouton était auparavant relâché, il vient d'être pressé
        //     if (!button_was_pressed) 
        //     {
        //         // Inverse l'état de la LED
        //         led1 = !led1;
        //         if (led1)
        //         {
        //             buzzer = 1;
        //         }
        //         else
        //         {
        //             buzzer = 0;
        //         }

        //         // Marque le bouton comme pressé
        //         button_was_pressed = true;
        //     }
        // }

        // // Vérifie si le bouton est relâché (état haut)
        // if (button == 1) 
        // {
        //     // Réinitialise l'état du bouton
        //     button_was_pressed = false;
        // }

        // Attente pour décharger le processeur (50 ms)
        ThisThread::sleep_for(100ms);
        
    }
}