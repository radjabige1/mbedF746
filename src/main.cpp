#include <mbed.h>
#include <threadLvgl.h>
#include <InterruptIn.h>
#include "demos/lv_demos.h"
#include <cstdio>

ThreadLvgl threadLvgl(30);

I2C my_i2c(D14, D15); // SDA, SCL
const int SRF08_ADDRESS = 0xE0;

// Configuration des LEDs
DigitalOut led_(D13);

// Configuration du buzzer
PwmOut buzzer(D10);
void sound_buzzer(int dist);



int main() 
{
    char init_tab[2] = {0x00, 0x51};
    char data[2];
    char reg_data_dist[1]={0x02}; // Registre de données de distance
    int dist;
    my_i2c.frequency(100000);

    while (true) 
    {
        my_i2c.write(SRF08_ADDRESS, init_tab, 2);
        ThisThread::sleep_for(100ms);
        my_i2c.write(SRF08_ADDRESS, reg_data_dist, 1);
        my_i2c.read(SRF08_ADDRESS, data, 2);
        dist= (data[0] << 8) + data[1];
        printf("Distance: %d cm\n", dist);
        sound_buzzer(dist);
        ThisThread::sleep_for(50ms);
    }
}

void sound_buzzer(int dist)
{
    if (dist >= 50) 
    {
        buzzer = 0;
    } 
    else if (dist <= 10) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(50ms);
        buzzer = 0;
    }
    else if (dist <= 20) 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(150ms);
        buzzer = 0;
    }  
    else 
    {
        buzzer.period(1.0 / 1000.0); // Fréquence de 1 kHz
        buzzer = 0.5; // 50% duty cycle
        ThisThread::sleep_for(500ms);
        buzzer = 0;
    }
}