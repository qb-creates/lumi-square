#ifndef BUTTONS_H
#define BUTTONS_H

#include <avr/io.h>
#include <util/delay.h>

class Input
{
public:
    static void configureButtonPins();
    static void updateSystemButtonStates();
    static void scanButtonMatrix();
    static bool getButton(uint8_t buttonIndex);
    static bool getButtonDown(uint8_t buttonIndex);
    static bool getNextButton();
    static bool getNextButtonUp();
    static bool getNextButtonDown();
    static bool getPreviousButton();
    static bool getPreviousButtonUp();
    static bool getPreviousButtonDown();

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
    static volatile bool nextButtonPressed;
    static volatile bool nextButtonUp;
    static volatile bool nextButtonDown;
    static volatile bool previousButtonPressed;
    static volatile bool previousButtonUp;
    static volatile bool previousButtonDown;
    static Button buttons[];
};
#endif