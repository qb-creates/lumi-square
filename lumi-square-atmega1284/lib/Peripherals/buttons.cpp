#include "buttons.h"
const uint32_t Input::rowOneBaseAddress = 0x0001;
const uint32_t Input::rowTwoBaseAddress = 0x0010;
const uint32_t Input::rowThreeBaseAddress = 0x0100;
const uint32_t Input::rowFourBaseAddress = 0x1000;
volatile bool Input::sleepButtonPressed = false;
volatile bool Input::sleepButtonUp = false;
volatile bool Input::muteButtonPressed = false;
volatile bool Input::muteButtonDown = false;
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
 * @brief Configures pins for system buttons and button matrix.
 * 
 * This function configures PD2 and PD3 as inputs for the sleep and mute buttons, respectively.
 * Additionally, it configures DDRA as inputs and outputs for the button matrix.
 * 
 * @return void
 */
void Input::configureButtonPins()
{
    // Enable pullup resistor for sleep and mute buttons
    DDRD &= ~(_BV(PD2) | _BV(PD3));
    PORTD |= _BV(PD2) | _BV(PD3);

    // Configure output/inputs for button matrix
    DDRA = 0x0F;
    PORTA |= 0xF0;
}

/**
 * @brief Updates the states of system buttons for the current frame.
 * 
 * This function updates boolean values to reflect the current status of all system buttons for 
 * the current frame. It checks if the buttons have been pressed down or released in the current 
 * frame and updates their states accordingly.
 * 
 * It is important to call this function frequently or at the beginning of each frame to ensure 
 * proper detection of system button presses.
 * 
 * @return void
 */
void Input::updateSystemButtonStates()
{
    if (muteButtonPressed != !(PIND & _BV(PD3)))
    {
        muteButtonPressed = !(PIND & _BV(PD3));
        muteButtonDown = muteButtonPressed;
    }

    if (sleepButtonPressed != !(PIND & _BV(PD2)))
    {
        sleepButtonPressed = !(PIND & _BV(PD2));
        sleepButtonUp = !sleepButtonPressed;
    }
}

/**
 * @brief Clears the states of system buttons.
 * 
 * This function resets the states of all system buttons back to false. It ensures
 * that the button states are cleared and ready for the next frame.
 * 
 * It is important to call this function at the end of each frame to ensure that
 * the button states are properly reset for the next frame.
 * 
 * @return void
 */
void Input::clearSystemButtonStates()
{
    if (muteButtonDown)
    {
        muteButtonDown = false;
    }

    if (sleepButtonUp)
    {
        sleepButtonUp = false;
    }
}

/**
 * @brief Scans the button matrix to detect pressed buttons.
 * 
 * This function scans a button matrix connected to the microcontroller,
 * detecting if any buttons have been pressed. It iterates through the rows
 * and columns of the matrix to determine the status of each button.
 * 
 * It is important to call this function frequently to ensure proper 
 * detection of button presses.
 * 
 * @return void
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
 * @brief Checks if the button at the specified index is currently pressed.
 * 
 * This function takes an index parameter representing the position of the
 * button in a button array. It returns true if the button at the specified
 * index is currently pressed, indicating that the user is holding down the button.
 * Otherwise, it returns false.
 * 
 * @param buttonIndex The index of the button in the button array.
 * 
 * @return bool True if the button at the specified index is pressed, false otherwise.
 */
bool Input::getButton(uint8_t buttonIndex)
{
    return buttonData & _BV(buttonIndex);
}

/**
 * @brief Checks if the button at the specified index was just pressed down.
 * 
 * This function takes an index parameter representing the position of the
 * button in a button array. It returns true if the button at the specified
 * index was just pressed down, indicating a new press in the current frame.
 * Otherwise, it returns false until the button is released and pressed again.
 * 
 * @param buttonIndex The index of the button in the button array.
 * 
 * @return bool True if the button at the specified index was just pressed down,
 *              false otherwise.
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

/**
 * @brief Checks if the sleep button is currently pressed.
 * 
 * This function returns true if the sleep button is currently pressed,
 * indicating that the user wants to put the microcontroller to sleep.
 * Otherwise, it returns false.
 * 
 * @return bool True if the sleep button is pressed, false otherwise.
 */
bool Input::getSleepButton()
{
    return sleepButtonPressed;
}

/**
 * @brief Checks if the sleep button was just released.
 * 
 * This function returns true if the sleep button was just released
 * in the current frame, indicating a release since the last frame.
 * Otherwise, it returns false until the button is pressed and released again.
 * 
 * @return bool True if the sleep button was just released, false otherwise.
 */
bool Input::getSleepButtonUp()
{
    return sleepButtonUp;
}

/**
 * @brief Checks if the mute button was just pressed down.
 * 
 * This function returns true if the mute button was just pressed down
 * in the current frame, indicating a new press since the last frame.
 * Otherwise, it returns false until the button is released and pressed again.
 * 
 * @return bool True if the mute button was just pressed down, false otherwise.
 */
bool Input::getMuteButtonDown()
{
    return muteButtonDown;
}
