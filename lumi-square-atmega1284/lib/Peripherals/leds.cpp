#include "leds.h"

volatile uint16_t Output::ledColorData[4][4][8];
volatile uint8_t Output::currentLedIndex = 0;
volatile bool Output::isDisplayRefreshing = false;

Output::LED Output::leds[16] = {
    LED(0, 0, 0x0001),
    LED(0, 1, 0x0001),
    LED(0, 2, 0x0001),
    LED(0, 3, 0x0001),
    LED(1, 0, 0x0008),
    LED(1, 1, 0x0008),
    LED(1, 2, 0x0008),
    LED(1, 3, 0x0008),
    LED(2, 0, 0x0040),
    LED(2, 1, 0x0040),
    LED(2, 2, 0x0040),
    LED(2, 3, 0x0040),
    LED(3, 0, 0x0200),
    LED(3, 1, 0x0200),
    LED(3, 2, 0x0200),
    LED(3, 3, 0x0200)};

/**
 * @brief Static Output object. Used to control the leds under the buttons.
 *
 * @param r The row the led is in.
 * @param c The coloumn the led is in.
 * @param baseAddress The base address for the driver pins this led is connected to.
 */
Output::LED::LED(uint8_t r, uint8_t c, uint16_t baseAddress)
    : row(r), column(c), rAddress(baseAddress), gAddress(baseAddress << 1), bAddress(baseAddress << 2), color(), isLedOn(false), intensity(1) {}

/**
 * @brief Configures the led communication.
 *
 */
void Output::configureLeds()
{
    // Configure outputs for leds
    DDRB |= 0x0F;
    PORTB |= 0x01;

    // Configure spi for communication with led driver
    /* Set SS, SCK, and MOSI as outputs. Set MISO as input */
    DDRB |= _BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPSR = _BV(SPI2X);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(CPOL);
}

/**
 * @brief Refreshes the leds. Call this every 1ms to properly show all the led colors.
 * Will result in a 62.5hz refresh rate froe the leds
 *
 */
void Output::refreshLeds()
{
    isDisplayRefreshing = true;

    LED *led = &leds[currentLedIndex];

    PORTB = _BV(led->column);

    for (int colorDataIndex = 0; colorDataIndex < 8; ++colorDataIndex)
    {
        Output::transmitLedData(ledColorData[led->column][led->row][colorDataIndex] >> 8);
        Output::transmitLedData(ledColorData[led->column][led->row][colorDataIndex] & 0xFF);
        Output::latchLedData();
        
        for (int j = 0; j < _BV(colorDataIndex); ++j)
        {
            asm("NOP");
        }
    }

    Output::transmitLedData(0x00);
    Output::transmitLedData(0x00);
    Output::latchLedData();

    ++currentLedIndex;

    if (currentLedIndex > 15)
        currentLedIndex = 0;

    isDisplayRefreshing = false;
}

/**
 * @brief Returns the color of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to get the color of.
 * @return Color
 */
Color Output::getLedColor(uint8_t ledIndex)
{
    return leds[ledIndex].color;
}

/**
 * @brief Sets the color of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to set the color.
 * @param color The color to change the led to.
 */
void Output::setLedColor(uint8_t ledIndex, const Color &color)
{
    LED *led = &leds[ledIndex];
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
void Output::setLedColor(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &leds[ledIndex];
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
void Output::ledOn(uint8_t ledIndex)
{
    LED *led = &leds[ledIndex];

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
void Output::ledOn(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &leds[ledIndex];
    led->color = color;
    led->isLedOn = true;
    led->intensity = intensity;
    onSetLedColor(*led, led->color, led->intensity);
}

void Output::ledOn(const uint8_t *range, uint8_t count, const Color &color, double intensity)
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
void Output::ledOff(uint8_t ledIndex)
{
    LED *led = &leds[ledIndex];

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
void Output::ledOff(uint8_t ledIndex, const Color &color, double intensity)
{
    LED *led = &leds[ledIndex];
    led->color = color;
    led->isLedOn = false;
    led->intensity = intensity;
    onSetLedColor(*led, Color(), 0);
}

void Output::ledOff(const uint8_t *range, uint8_t count, const Color &color, double intensity)
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
bool Output::getLedStatus(uint8_t ledIndex)
{
    return leds[ledIndex].isLedOn;
}

/**
 * @brief
 *
 * @param ledIndex Index of the led you want to get the power status for.
 * @return uint8_t
 */
double Output::getLedIntensity(uint8_t ledIndex)
{
    return leds[ledIndex].intensity;
}

/**
 * @brief Sets the intensity of the led identified by the ledIndex.
 *
 * @param ledIndex Index of the led you want to set the intensity for
 * @param intensity The intensity level to set the led to. Valid values are from 0 - 1.
 */
void Output::setLedIntensity(uint8_t ledIndex, double intensity)
{
    LED *led = &leds[ledIndex];

    if (led->isLedOn)
    {
        onSetLedColor(*led, led->color, intensity);
    }

    led->intensity = intensity;
}

void Output::enableSimonLights(bool all)
{
    uint8_t gameSelectButtons[] = {0, 1, 2, 3, 4, 7, 8, 11, 12, 13, 14, 15};
    Output::ledOff(gameSelectButtons, 12, Colors::azure, .5);

    if (all)
    {
        Output::ledOn(0, Colors::aquamarine, .1);
        Output::ledOn(3, Colors::purple, .1);
        Output::ledOn(12, Colors::orange, .1);
        Output::ledOn(15, Colors::azure, .1);
    }

    Output::ledOn(5, Colors::green, .1);
    Output::ledOn(6, Colors::red, .1);
    Output::ledOn(9, Colors::yellow, .1);
    Output::ledOn(10, Colors::blue, .1);
}

void Output::enableMemoryMatchLights()
{
    Output::ledOn(0, Colors::red, .4);
    Output::ledOn(1, Colors::pink, .4);
    Output::ledOn(2, Colors::cyan, .4);
    Output::ledOn(3, Colors::azure, .4);
    Output::ledOn(4, Colors::orange, .4);
    Output::ledOn(5, Colors::aquamarine, .4);
    Output::ledOn(6, Colors::cyan, .4);
    Output::ledOn(7, Colors::pink, .4);
    Output::ledOn(8, Colors::yellow, .4);
    Output::ledOn(9, Colors::purple, .4);
    Output::ledOn(10, Colors::orange, .4);
    Output::ledOn(11, Colors::azure, .4);
    Output::ledOn(12, Colors::purple, .4);
    Output::ledOn(13, Colors::red, .4);
    Output::ledOn(14, Colors::aquamarine, .4);
    Output::ledOn(15, Colors::yellow, .4);
}

void Output::onSetLedColor(const LED &led, const Color &color, double intensity)
{
    while (isDisplayRefreshing)
    {
    }

    cli();

    uint8_t column = led.column;
    uint8_t row = led.row;

    for (int i = 0; i < 8; i++)
    {
        uint8_t mask = _BV(i);

        ledColorData[column][row][i] &= ~(led.rAddress | led.gAddress | led.bAddress);
        ledColorData[column][row][i] |= ((uint8_t)(color.r * intensity) & mask) ? led.rAddress : 0;
        ledColorData[column][row][i] |= ((uint8_t)(color.g * intensity) & mask) ? led.gAddress : 0;
        ledColorData[column][row][i] |= ((uint8_t)(color.b * intensity) & mask) ? led.bAddress : 0;
    }

    sei();
}

void Output::transmitLedData(char dData)
{
    /* Start transmission */
    SPDR = dData;

    /* Wait for transmission complete */
    loop_until_bit_is_set(SPSR, SPIF);
}

void Output::latchLedData()
{
    PORTB |= _BV(PB4);
    PORTB &= ~_BV(PB4);
}