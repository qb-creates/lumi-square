#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr/io.h>
#include <util/delay.h>

class Input
{
public:
    static void configureButtonMatrix();
    static void scanButtonMatrix();
    static bool getButton(uint8_t buttonIndex);
    static bool getButtonDown(uint8_t buttonIndex);

private:
    class Button
    {
    public:
        Button(uint8_t buttonIndex);
        const uint32_t address;
        bool pressed;
    };

private:
    Input();
    static const uint32_t rowOneBaseAddress;
    static const uint32_t rowTwoBaseAddress;
    static const uint32_t rowThreeBaseAddress;
    static const uint32_t rowFourBaseAddress;
    static volatile uint32_t buttonData;
    static Button buttons[];
};
#endif