#include <mbed.h>
#include <threadLvgl.h>
#include <threadLedsNeo.h>

#include <cstdio>

ThreadLvgl threadLvgl(30);
ThreadLedsNeo threadLeds(A3, 5);

int main() {

    threadLvgl.lock();

    threadLvgl.unlock();


    int n = 0;

    while (1) {
        // put your main code here, to run repeatedly:
        threadLeds.lock();
        threadLeds.setLed(n, 0,0,0);
        n++;
        if (n>=30) n=0;
        threadLeds.setLed(n, 255,0,0);
        threadLeds.unlock();

        ThisThread::sleep_for(100ms);
    }
}

