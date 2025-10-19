#ifndef LEDS_H
#define LEDS_H
#include "../Colors/colors.h"
// #include <avr/interrupt.h>
// #include <util/delay.h>

class LED
{
public:
    static LED leds[];
public:
    LED();
    LED(uint8_t r, uint8_t c, uint16_t baseAddress);
    const uint8_t row;
    const uint8_t column;
    const uint16_t rAddress;
    const uint16_t gAddress;
    const uint16_t bAddress;
    Color color;
    bool isLedOn;
    double intensity;
};

#endif