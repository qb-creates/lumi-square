#ifndef LEDS_H
#define LEDS_H
#include "../Colors/colors.h"
#include "iostream.h"
#include <avr/interrupt.h>
#include <util/delay.h>

class Output
{
public:
    static void refreshLeds();
    static void configureLeds();
    static Color getLedColor(uint8_t ledIndex);
    static void setLedColor(uint8_t ledIndex, const Color &color);
    static void setLedColor(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOn(uint8_t ledIndex);
    static void ledOn(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOn(const uint8_t * range, uint8_t count, const Color &color, double intensity);
    static void ledOff(uint8_t ledIndex);
    static void ledOff(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOff(const uint8_t * range, uint8_t count, const Color &color, double intensity);
    static bool getLedStatus(uint8_t ledIndex);
    static double getLedIntensity(uint8_t ledIndex);
    static void setLedIntensity(uint8_t ledIndex, double intensity);
    static void enableSimonLights(bool all);
    static void enableMemoryMatchLights();

private:
    class LED
    {
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

private:
    Output();
    static void onSetLedColor(const LED &led, const Color &color, double intensity);
    static volatile uint16_t ledColorData[4][4][8];
    static volatile uint8_t currentLedIndex;
    static volatile bool isDisplayRefreshing;
    static LED leds[];
};

#endif