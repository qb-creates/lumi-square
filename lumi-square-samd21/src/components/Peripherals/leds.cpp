#include "leds.h"
#include "samd21j18a.h"

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
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB00 | PORT_PB01 | PORT_PB02 | PORT_PB03;
    PORT_REGS->GROUP[1].PORT_OUT &= ~(PORT_PB00 | PORT_PB01 | PORT_PB02 | PORT_PB03);

    // Configure spi for communication with led driver
    // Enable gpio multiplexer for pb12 and pb13.
    PORT_REGS->GROUP[1].PORT_PINCFG[12] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[1].PORT_PINCFG[13] = PORT_PINCFG_PMUXEN_Msk;

    // Select peripheral function C for pb12, pb13. pb12 will be Dout and pb 13 will be the clock
    PORT_REGS->GROUP[1].PORT_PMUX[6] = PORT_PMUX_PMUXO_C | PORT_PMUX_PMUXE_C;

    // configure pb14 to be an output. Will control the latch in software
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB14;
    PORT_REGS->GROUP[1].PORT_OUT &= ~PORT_PB14;

    // Turn on clock for SERCOM4
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM4_Msk;

    // Configure sercom 4 to use clock generator 0
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM4_CORE | GCLK_CLKCTRL_CLKEN_Msk;

    // Gives us a baud rate of 1Mhz
    SERCOM4_REGS->SPIM.SERCOM_BAUD = 2;

    // Enable hardware SS control. Character sizse is set to 8 bits
    SERCOM4_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT;
    while ((SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_CTRLB_Msk) == SERCOM_SPIM_SYNCBUSY_CTRLB_Msk)
    {
    }

    // Set sercom 4 to spi master. Transmit msb first. Set to SPI frame. Configure pads.
    SERCOM4_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE_Msk | SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | SERCOM_SPIM_CTRLA_DORD_MSB | SERCOM_SPIM_CTRLA_FORM_SPI_FRAME | SERCOM_SPIM_CTRLA_DIPO_PAD3 | SERCOM_SPIM_CTRLA_DOPO_PAD0 | SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH;
    while ((SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk) == SERCOM_SPIM_SYNCBUSY_ENABLE_Msk)
    {
    }
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
    PORT_REGS->GROUP[1].PORT_OUT = (PORT_REGS->GROUP[1].PORT_OUT & 0xFFFFFFF0) | (1 << led->column);
    
    for (int colorDataIndex = 0; colorDataIndex < 8; ++colorDataIndex)
    {        
        Output::transmitLedData(ledColorData[led->column][led->row][colorDataIndex]);        
        Output::latchLedData();

        for (int j = 0; j < (1 << colorDataIndex); ++j)
        {
            __NOP();
        }
    }

    Output::transmitLedData(0x0000);
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

    __disable_irq();

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

    __enable_irq();
}

void Output::transmitLedData(uint16_t data)
{    
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk)) { }
    SERCOM4_REGS->SPIM.SERCOM_DATA = data >> 8;

    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk)) { }
    SERCOM4_REGS->SPIM.SERCOM_DATA = data & 0xFF;
}

void Output::latchLedData()
{
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB14;
    PORT_REGS->GROUP[1].PORT_OUT &= ~PORT_PB14;
}