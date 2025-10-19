#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include "led.h"

class LEDMatrix
{
private:
    static volatile uint16_t ledColorData[4][4][8];

public:
    static void refreshLeds(void);
    static Color getLedColor(uint8_t ledIndex);
    static void setLedColor(uint8_t ledIndex, const Color &color);
    static void setLedColor(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOn(uint8_t ledIndex);
    static void ledOn(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOn(const uint8_t *range, uint8_t count, const Color &color, double intensity);
    static void ledOff(uint8_t ledIndex);
    static void ledOff(uint8_t ledIndex, const Color &color, double intensity);
    static void ledOff(const uint8_t *range, uint8_t count, const Color &color, double intensity);
    static bool getLedStatus(uint8_t ledIndex);
    static double getLedIntensity(uint8_t ledIndex);
    static void setLedIntensity(uint8_t ledIndex, double intensity);
    static void enableSimonLights(bool all);
    static void enableMemoryMatchLights();

private:
    LEDMatrix() = delete;
    LEDMatrix(const LEDMatrix &) = delete;
    void operator=(const LEDMatrix &) = delete;
    static void onSetLedColor(const LED &led, const Color &color, double intensity);
};

#endif