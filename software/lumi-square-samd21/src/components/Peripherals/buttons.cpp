#include "buttons.h"
#include "deviceutility.h"

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

void Input::pollButtons()
{
    buttonData = DeviceUtility::Instance().scanButtonMatrix();

    if (previousButtonPressed != DeviceUtility::Instance().getPreviousButtonState())
    {
        previousButtonPressed = DeviceUtility::Instance().getPreviousButtonState();
        previousButtonDown = previousButtonPressed;
        previousButtonUp = !previousButtonPressed;
    }
    
    if (nextButtonPressed != DeviceUtility::Instance().getNextButtonState())
    {
        nextButtonPressed = DeviceUtility::Instance().getNextButtonState();
        nextButtonDown = nextButtonPressed;
        nextButtonUp = !nextButtonPressed;
    }
    
    if (difficultyButtonPressed != DeviceUtility::Instance().getDifficultyButtonState())
    {
        difficultyButtonPressed = DeviceUtility::Instance().getDifficultyButtonState();
        difficultyButtonDown = difficultyButtonPressed;
        difficultyButtonUp = !difficultyButtonPressed;
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
