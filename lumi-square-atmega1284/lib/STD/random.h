#ifndef RANDOM_H
#define RANDOM_H

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

class Random
{
public:
    static void configureRNG();
    static void seedRNG();
    static int16_t range(int16_t min = 1, int16_t max = 255);

private:
    Random();
};

#endif