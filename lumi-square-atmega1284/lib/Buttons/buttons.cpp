#include "buttons.h"
const uint32_t Input::rowOneBaseAddress = 0x0001;
const uint32_t Input::rowTwoBaseAddress = 0x0010;
const uint32_t Input::rowThreeBaseAddress = 0x0100;
const uint32_t Input::rowFourBaseAddress = 0x1000;
volatile uint32_t Input::buttonData = 0;

Input::Button Input::buttons[16] = {
    Button(0),
    Button(1),
    Button(2),
    Button(3),
    Button(4),
    Button(5),
    Button(6),
    Button(7),
    Button(8),
    Button(9),
    Button(10),
    Button(11),
    Button(12),
    Button(13),
    Button(14),
    Button(15)};

/**
 * @brief Constructs a new Button object
 *
 * @param buttonIndex The index/position of the button. 0 is the top left button and 15 is the bottom right.
 */
Input::Button::Button(uint8_t buttonIndex)
    : address(_BV(buttonIndex)), pressed(false) {}

/**
 * @brief Configures the input and outputs for the button matrix
 * 
 */
void Input::configureButtonMatrix()
{
    DDRA = 0x0F;
    PORTA |= 0xF0;
}

/**
 * @brief Scan the button matrix. Call this periodically to update the button pressed states.
 *
 */
void Input::scanButtonMatrix()
{
    buttonData = 0;

    for (int columnAddress = 0x01; columnAddress < 0x10; columnAddress <<= 1)
    {
        PORTA = ~columnAddress;
        _delay_us(3);

        if (!(PINA & _BV(PIN4)))
            buttonData |= rowOneBaseAddress * columnAddress;

        if (!(PINA & _BV(PIN5)))
            buttonData |= rowTwoBaseAddress * columnAddress;

        if (!(PINA & _BV(PIN6)))
            buttonData |= rowThreeBaseAddress * columnAddress;

        if (!(PINA & _BV(PIN7)))
            buttonData |= rowFourBaseAddress * columnAddress;
    }
}

/**
 * @brief Returns true while the button identified by the buttonIndex is held down
 *
 * @param buttonIndex Index of the button you want to get the pressed state for
 * @return true
 * @return false
 */
bool Input::getButton(uint8_t buttonIndex)
{
    return buttonData & _BV(buttonIndex);
}

/**
 * @brief Returns true during the frame the user pressed down the button identified by the buttonIndex.
 *
 * @param buttonIndex Index of the button you want to get the pressed state for
 * @return true
 * @return false
 */
bool Input::getButtonDown(uint8_t buttonIndex)
{
    Button *button = &buttons[buttonIndex];

    if (!button->pressed && (buttonData & button->address))
    {
        button->pressed = true;
        return true;
    }
    else if (!(buttonData & button->address))
    {
        button->pressed = false;
    }

    return false;
}