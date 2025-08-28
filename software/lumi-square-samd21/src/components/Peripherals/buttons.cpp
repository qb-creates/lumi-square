#include "buttons.h"
#include "samd21j18a.h"

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
    : address((1 << buttonIndex)), pressed(false) {}

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
    // Enable input and pull up resisotrs
    PORT_REGS->GROUP[1].PORT_PINCFG[16] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[17] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[22] |= 0x06;

    // Configure as input and connect to pull up resistor
    PORT_REGS->GROUP[1].PORT_DIR &= ~(PORT_PB16 | PORT_PB17 | PORT_PB22);
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB16 | PORT_PB17 | PORT_PB22;

    // Configure output/inputs for button matrix
    // Enanle input and pull up resistors
    PORT_REGS->GROUP[1].PORT_PINCFG[8] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[9] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[10] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[11] |= 0x06;

    // Configure as input and connect to pull up resistor
    PORT_REGS->GROUP[1].PORT_DIR &= ~(PORT_PB08 | PORT_PB09 | PORT_PB10 | PORT_PB11);
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB08 | PORT_PB09 | PORT_PB10 | PORT_PB11;

    // Configure as outputs
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB04 | PORT_PB05 | PORT_PB06 | PORT_PB07;
    PORT_REGS->GROUP[1].PORT_OUT &= ~(PORT_PB04 | PORT_PB05 | PORT_PB06 | PORT_PB07);
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
    if (previousButtonPressed != !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB16))
    {
        previousButtonPressed = !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB16);
        previousButtonDown = previousButtonPressed;
        previousButtonUp = !previousButtonPressed;
    }

    if (nextButtonPressed != !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB17))
    {
        nextButtonPressed = !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB17);
        nextButtonDown = nextButtonPressed;
        nextButtonUp = !nextButtonPressed;
    }

    if (difficultyButtonPressed != !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB22))
    {
        difficultyButtonPressed = !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB22);
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

    for (uint16_t columnAddress = 0x10; columnAddress < 0x100; columnAddress <<= 1)
    {
        PORT_REGS->GROUP[1].PORT_OUT = (PORT_REGS->GROUP[1].PORT_OUT & 0xFFFFFF0F) | ~columnAddress;

        for (int i = 0; i < 144; i++)
        {
            __NOP();
        }

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB08))
            buttonData |= rowOneBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB09))
            buttonData |= rowTwoBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB10))
            buttonData |= rowThreeBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB11))
            buttonData |= rowFourBaseAddress * (columnAddress >> 4);
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
    return buttonData & (1 << buttonIndex);
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
