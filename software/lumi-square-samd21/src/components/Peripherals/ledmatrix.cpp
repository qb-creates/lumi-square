#include "ledmatrix.h"
#include "deviceutility.h"

volatile uint16_t LEDMatrix::ledColorData[4][4][8];

/**
 * @brief Refreshes the leds. Call this every 1ms to properly show all the led colors.
 * Will result in a 62.5hz refresh rate froe the leds
 *
 */
void LEDMatrix::refreshLeds()
{
    DeviceUtility::Instance().refreshButtonColor(ledColorData);
}

/**
 * @brief Returns the color of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to get the color of.
 * @return Color
 */
Color LEDMatrix::getLedColor(uint8_t ledIndex)
{
    return LED::leds[ledIndex].color;
}

/**
 * @brief Sets the color of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to set the color.
 * @param color The color to change the led to.
 */
void LEDMatrix::setLedColor(uint8_t ledIndex, const Color &color)
{
    LED *led = &LED::leds[ledIndex];
    led->color = color;

    if (led->isLedOn)
    {
        onSetLedColor(*led, color, led->intensity);
    }
}

/**
 * @brief Sets the color of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to set the color.
 * @param color The color to change the led to.
 * @param intensity The intensity level to set the led to. Valid values are from 0 - 1.
 */
void LEDMatrix::setLedColor(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &LED::leds[ledIndex];
    led->color = color;

    if (led->isLedOn)
    {
        onSetLedColor(*led, color, led->intensity);
    }

    setLedIntensity(ledIndex, intensity);
}

/**
 * @brief Turns on the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to turn on.
 */
void LEDMatrix::ledOn(uint8_t ledIndex)
{
    LED *led = &LED::leds[ledIndex];

    if (!led->isLedOn)
    {
        onSetLedColor(*led, led->color, led->intensity);

        led->isLedOn = true;
    }
}

/**
 * @brief Turns on the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to turn on.
 * @param color The color to change the led to.
 * @param intensity The intensity level to set the led to. Valid values are from 0 - 1.
 */
void LEDMatrix::ledOn(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &LED::leds[ledIndex];
    led->color = color;
    led->isLedOn = true;
    led->intensity = intensity;
    onSetLedColor(*led, led->color, led->intensity);
}

void LEDMatrix::ledOn(const uint8_t *range, uint8_t count, const Color &color, double intensity)
{
    for (int i = 0; i < count; ++i)
    {
        ledOn(range[i], color, intensity);
    }
}

/**
 * @brief Turns off the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to turn off.
 */
void LEDMatrix::ledOff(uint8_t ledIndex)
{
    LED *led = &LED::leds[ledIndex];

    if (led->isLedOn)
    {
        onSetLedColor(*led, Color(), 0);

        led->isLedOn = false;
    }
}

/**
 * @brief Turns off the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to turn on.
 * @param color The color to change the led to.
 * @param intensity The intensity level to set the led to. Valid values are from 0 - 1.
 */
void LEDMatrix::ledOff(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &LED::leds[ledIndex];
    led->color = color;
    led->isLedOn = false;
    led->intensity = intensity;
    onSetLedColor(*led, Color(), 0);
}

void LEDMatrix::ledOff(const uint8_t *range, uint8_t count, const Color &color, double intensity)
{
    for (int i = 0; i < count; ++i)
    {
        ledOff(range[i], color, intensity);
    }
}

/**
 * @brief Gets the status of the led identified by the ledIndex. Will return true if led is on. False if it is off
 *
 * @param ledIndex Index of the led you want to get the power status for.
 * @return true
 * @return false
 */
bool LEDMatrix::getLedStatus(uint8_t ledIndex)
{
    return LED::leds[ledIndex].isLedOn;
}

/**
 * @brief
 *
 * @param ledIndex Index of the led you want to get the power status for.
 * @return uint8_t
 */
double LEDMatrix::getLedIntensity(uint8_t ledIndex)
{
    return LED::leds[ledIndex].intensity;
}

/**
 * @brief Sets the intensity of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to set the intensity for
 * @param intensity The intensity level to set the led to. Valid values are from 0 - 1.
 */
void LEDMatrix::setLedIntensity(uint8_t ledIndex, double intensity)
{
    LED *led = &LED::leds[ledIndex];

    if (led->isLedOn)
    {
        onSetLedColor(*led, led->color, intensity);
    }

    led->intensity = intensity;
}

void LEDMatrix::enableSimonLights(bool all)
{
    uint8_t gameSelectButtons[] = {0, 1, 2, 3, 4, 7, 8, 11, 12, 13, 14, 15};
    LEDMatrix::ledOff(gameSelectButtons, 12, Colors::turquoise, .5);

    if (all)
    {
        LEDMatrix::ledOn(0, Colors::purple, .5);
        LEDMatrix::ledOn(3, Colors::pink, .5);
        LEDMatrix::ledOn(12, Colors::orange, .5);
        LEDMatrix::ledOn(15, Colors::turquoise, .5);
    }

    LEDMatrix::ledOn(5, Colors::green, .5);
    LEDMatrix::ledOn(6, Colors::red, .5);
    LEDMatrix::ledOn(9, Colors::yellow, .5);
    LEDMatrix::ledOn(10, Colors::blue, .5);
}

void LEDMatrix::enableMemoryMatchLights()
{
    LEDMatrix::ledOn(0, Colors::red, .4);
    LEDMatrix::ledOn(1, Colors::pink, .4);
    LEDMatrix::ledOn(2, Colors::blue, .4);
    LEDMatrix::ledOn(3, Colors::turquoise, .4);
    LEDMatrix::ledOn(4, Colors::orange, .4);
    LEDMatrix::ledOn(5, Colors::green, .4);
    LEDMatrix::ledOn(6, Colors::blue, .4);
    LEDMatrix::ledOn(7, Colors::pink, .4);
    LEDMatrix::ledOn(8, Colors::yellow, .4);
    LEDMatrix::ledOn(9, Colors::purple, .4);
    LEDMatrix::ledOn(10, Colors::orange, .4);
    LEDMatrix::ledOn(11, Colors::turquoise, .4);
    LEDMatrix::ledOn(12, Colors::purple, .4);
    LEDMatrix::ledOn(13, Colors::red, .4);
    LEDMatrix::ledOn(14, Colors::green, .4);
    LEDMatrix::ledOn(15, Colors::yellow, .4);
}

void LEDMatrix::onSetLedColor(const LED &led, const Color &color, double intensity)
{
    uint8_t column = led.column;
    uint8_t row = led.row;

    for (int i = 0; i < 8; i++)
    {
        uint8_t mask = (1 << i);

        ledColorData[column][row][i] &= ~(led.rAddress | led.gAddress | led.bAddress);
        ledColorData[column][row][i] |= ((uint8_t)(color.r * intensity) & mask) ? led.rAddress : 0;
        ledColorData[column][row][i] |= ((uint8_t)(color.g * intensity) & mask) ? led.gAddress : 0;
        ledColorData[column][row][i] |= ((uint8_t)(color.b * intensity) & mask) ? led.bAddress : 0;
    }
}