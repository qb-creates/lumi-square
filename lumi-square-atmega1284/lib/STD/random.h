#ifndef RANDOM_H
#define RANDOM_H

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>

class Random
{
public:
    static void configureRNG();
    static uint8_t range(uint8_t min = 1, uint8_t max = 255);

private:
    Random();
};

#endif