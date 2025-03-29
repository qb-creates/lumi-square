#include "buttons.h"
const uint32_t Input::rowOneBaseAddress = 0x0001;
const uint32_t Input::rowTwoBaseAddress = 0x0010;
const uint32_t Input::rowThreeBaseAddress = 0x0100;
const uint32_t Input::rowFourBaseAddress = 0x1000;
volatile bool Input::nextButtonPressed = false;
volatile bool Input::nextButtonUp = false;
volatile bool Input::nextButtonDown = false;
volatile bool Input::previousButtonPressed = false;
volatile bool Input::previousButtonUp = false;
volatile bool Input::previousButtonDown = false;
volatile bool Input::difficultyButtonPressed = false;
volatile bool Input::difficultyButtonUp = false;
volatile bool Input::difficultyButtonDown = false;
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
 * This function configures PC4 and PC5 as inputs for the next and previous buttons, respectively.
 * Additionally, it configures DDRA as inputs and outputs for the button matrix.
 *
 * @return void
 */
void Input::configureButtonPins()
{
    // Enable pullup resistor for next and previous buttons
    DDRD &= ~( _BV(PD7));
    PORTD |= _BV(PD7);

    DDRC &= ~(_BV(PC4) | _BV(PC5));
    PORTC |= _BV(PC4) | _BV(PC5);

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
    if (previousButtonPressed != !(PINC & _BV(PD5)))
    {
        previousButtonPressed = !(PINC & _BV(PD5));
        previousButtonDown = previousButtonPressed;
    }

    if (nextButtonPressed != !(PINC & _BV(PD4)))
    {
        nextButtonPressed = !(PINC & _BV(PD4));
        nextButtonDown = nextButtonPressed;
        nextButtonUp = !nextButtonPressed;
    }

    if (difficultyButtonPressed != !(PIND & _BV(PD7)))
    {
        difficultyButtonPressed = !(PIND & _BV(PD7));
        difficultyButtonDown = difficultyButtonPressed;
        difficultyButtonUp = !difficultyButtonPressed;
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
 * @brief Checks if the next button is currently pressed.
 *
 * This function returns true if the next button is currently pressed,
 *
 * @return bool True if the next button is pressed, false otherwise.
 */
bool Input::getNextButton()
{
    return nextButtonPressed;
}

/**
 * @brief Checks if the next button was just released.
 *
 * This function returns true if the next button was just released
 * in the current frame, indicating a release since the last frame.
 * Otherwise, it returns false until the button is pressed and released again.
 *
 * @return bool True if the next button was just released, false otherwise.
 */
bool Input::getNextButtonUp()
{
    bool buttonState = nextButtonUp;
    nextButtonUp = false;
    return buttonState;
}

/**
 * @brief Checks if the next button was just pressed down.
 *
 * This function returns true if the next button was just pressed down
 * in the current frame, indicating a new press since the last frame.
 * Otherwise, it returns false until the button is released and pressed again.
 *
 * @return bool True if the next button was just pressed down, false otherwise.
 */
bool Input::getNextButtonDown()
{
    bool buttonState = nextButtonDown;
    nextButtonDown = false;
    return buttonState;
}

/**
 * @brief Checks if the previous button is currently pressed.
 *
 * This function returns true if the previous button is currently pressed.
 *
 * @return bool True if the previous button is pressed, false otherwise.
 */
bool Input::getPreviousButton()
{
    return previousButtonPressed;
}

bool Input::getPreviousButtonUp()
{
    bool buttonState = previousButtonUp;
    previousButtonUp = false;
    return buttonState;
}

/**
 * @brief Checks if the previous button was just pressed down.
 *
 * This function returns true if the previous button was just pressed down
 * in the current frame, indicating a new press since the last frame.
 * Otherwise, it returns false until the button is released and pressed again.
 *
 * @return bool True if the previous button was just pressed down, false otherwise.
 */
bool Input::getPreviousButtonDown()
{
    bool buttonState = previousButtonDown;
    previousButtonDown = false;
    return buttonState;
}

bool Input::getDifficultyButton()
{
    return difficultyButtonPressed;
}

bool Input::getDifficultyButtonUp()
{
    bool buttonState = difficultyButtonUp;
    difficultyButtonUp = false;
    return buttonState;
}

bool Input::getDifficultyButtonDown()
{
    bool buttonState = difficultyButtonDown;
    difficultyButtonDown = false;
    return buttonState;
}
