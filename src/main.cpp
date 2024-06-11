#include <mbed.h>
#include <threadLvgl.h> // Inclusion de la bibliothèque pour le thread LVGL
#include <cstdio> // Inclusion pour les entrées/sorties formatées (stdio)
#include <radar.h> // Inclusion de la bibliothèque pour le radar (non spécifiée dans le code)

// Configuration I2C pour le capteur SRF08
I2C my_i2c(D14, D15); // Déclaration et initialisation d'un objet I2C avec les broches D14 et D15 pour SDA et SCL respectivement
const int SRF08_ADDRESS = 0xE0; // Adresse I2C du capteur SRF08

// Configuration du buzzer
PwmOut buzzer(D10); // Déclaration et initialisation d'un objet PwmOut pour le buzzer avec la broche D10

// Variables de contrôle
bool radar_active = false; // Variable booléenne pour indiquer si le radar est actif ou non

// Initialisation du thread LVGL
ThreadLvgl threadLvgl(30); // Déclaration et initialisation d'un objet ThreadLvgl avec une priorité de 30

int main() 
{
    // Initialisation LVGL et création de l'interface utilisateur
    threadLvgl.lock(); // Verrouillage du thread LVGL
    create_ui(); // Appel d'une fonction pour créer l'interface utilisateur 
    threadLvgl.unlock(); // Déverrouillage du thread LVGL

    // Configuration I2C
    char init_tab[2] = {0x00, 0x51}; // Tableau d'initialisation pour le capteur SRF08
    char data[2]; // Tableau pour stocker les données lues depuis le capteur
    char reg_data_dist[1] = {0x02}; // Registre de données de distance pour le capteur SRF08
    int dist; // Variable pour stocker la distance mesurée
    my_i2c.frequency(100000); // Définition de la fréquence de communication I2C à 100 kHz

    while (true) // Boucle principale du programme
    {
        if (radar_active) // Vérification si le radar est actif
        {
            my_i2c.write(SRF08_ADDRESS, init_tab, 2); // Envoi des données d'initialisation au capteur SRF08
            ThisThread::sleep_for(100ms); // Attente de 100 millisecondes
            my_i2c.write(SRF08_ADDRESS, reg_data_dist, 1); // Envoi de la demande de lecture de la distance au capteur
            my_i2c.read(SRF08_ADDRESS, data, 2); // Lecture des données de distance depuis le capteur
            dist = (data[0] << 8) + data[1]; // Calcul de la distance à partir des données lues
            printf("Distance: %d cm\n", dist); // Affichage de la distance mesurée sur la console
            sound_buzzer(dist); // Appel d'une fonction pour activer le buzzer en fonction de la distance
            threadLvgl.lock(); // Verrouillage du thread LVGL
            update_progress_bar(dist); // Appel d'une fonction pour mettre à jour la barre de progression (non définie dans le code)
            update_distance_label(dist); // Appel d'une fonction pour mettre à jour l'étiquette de distance (non définie dans le code)
            threadLvgl.unlock(); // Déverrouillage du thread LVGL
        }
        ThisThread::sleep_for(200ms); // Attente de 200 millisecondes avant de continuer la boucle
    }
}
