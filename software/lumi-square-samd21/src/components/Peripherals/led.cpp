#include "led.h"

LED LED::leds[16] = {
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
 * @brief Static LEDMatrix object. Used to control the leds under the buttons.
 *
 * @param r The row the led is in.
 * @param c The coloumn the led is in.
 * @param baseAddress The base address for the driver pins this led is connected to.
 */
LED::LED(uint8_t r, uint8_t c, uint16_t baseAddress)
    : row(r), column(c), rAddress(baseAddress), gAddress(baseAddress << 1), bAddress(baseAddress << 2), color(), isLedOn(false), intensity(1) {}