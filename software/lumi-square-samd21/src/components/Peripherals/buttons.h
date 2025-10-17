#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

class Input
{
public:
    /**
     * @brief Polls all button inputs and updates their current states.
     *
     * This function performs a complete button input cycle by scanning the 4x4 button matrix
     * and reading the system buttons (Previous, Next, Difficulty). It updates the internal
     * button state variables that can be queried by other parts of the system.
     *
     * @note This function should be called regularly in the main loop to maintain responsive button input detection.
     *
     * @return void
     *
     */
    static void pollButtons();
    static bool getButton(uint8_t buttonIndex);
    static bool getButtonDown(uint8_t buttonIndex);
    static bool getNextButton();
    static bool getNextButtonUp();
    static bool getNextButtonDown();
    static bool getPreviousButton();
    static bool getPreviousButtonUp();
    static bool getPreviousButtonDown();
    static bool getDifficultyButton();
    static bool getDifficultyButtonUp();
    static bool getDifficultyButtonDown();

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
    static volatile uint32_t buttonData;
    static volatile bool nextButtonPressed;
    static volatile bool nextButtonUp;
    static volatile bool nextButtonDown;
    static volatile bool previousButtonPressed;
    static volatile bool previousButtonUp;
    static volatile bool previousButtonDown;
    static volatile bool difficultyButtonPressed;
    static volatile bool difficultyButtonUp;
    static volatile bool difficultyButtonDown;
    static Button buttons[];
};
#endif