#include <mbed.h>
#include <threadLvgl.h>
#include <cstdio>
#include <radar.h>


// Configuration I2C pour le capteur SRF08
I2C my_i2c(D14, D15); // SDA, SCL
const int SRF08_ADDRESS = 0xE0;

// Configuration du buzzer
PwmOut buzzer(D10);

// Variables de contrôle
bool radar_active = false;

// Initialisation du thread LVGL
ThreadLvgl threadLvgl(30);

int main() 
{
    // Initialisation LVGL et création de l'interface utilisateur
    threadLvgl.lock();
    create_ui();
    threadLvgl.unlock();

    // Configuration I2C
    char init_tab[2] = {0x00, 0x51};
    char data[2];
    char reg_data_dist[1] = {0x02}; // Registre de données de distance
    int dist;
    my_i2c.frequency(100000);

    while (true) 
    {
        if (radar_active) 
        {
            my_i2c.write(SRF08_ADDRESS, init_tab, 2);
            ThisThread::sleep_for(100ms);
            my_i2c.write(SRF08_ADDRESS, reg_data_dist, 1);
            my_i2c.read(SRF08_ADDRESS, data, 2);
            dist = (data[0] << 8) + data[1];
            printf("Distance: %d cm\n", dist);
            sound_buzzer(dist);
            threadLvgl.lock();
            update_progress_bar(dist);
            update_distance_label(dist);
            threadLvgl.unlock();
        }
        ThisThread::sleep_for(200ms);
    }
}
