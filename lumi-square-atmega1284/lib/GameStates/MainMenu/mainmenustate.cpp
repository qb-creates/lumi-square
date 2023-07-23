#include "mainmenustate.h"
#include "audio.h"
#include "leds.h"

MainMenuState::MainMenuState()
    : GameBaseState(), queuedState(GameState::MemoryMatching), currentSelectedButton(0), flashSelectedButtonTimer(0), flashTargetTime(800) {}

void MainMenuState::enterState()
{
    for (int i = 1; i < 8; i++)
    {
        Output::ledOn(i);
        Output::setLedColor(i, Colors::azure);
        Output::setLedIntensity(i, .05);
    }

    Output::ledOn(0);
    Output::setLedColor(0, Colors::yellow);
    Output::setLedIntensity(0, .5);

    Output::ledOn(12);
    Output::setLedColor(12, Colors::orange);
    Output::setLedIntensity(12, 1);

    Output::ledOn(15);
    Output::setLedColor(15, Colors::green);
    Output::setLedIntensity(15, 1);
}

void MainMenuState::exitState()
{
    for (int i = 0; i < 16; i++)
    {
        Output::ledOff(i);
    }

    currentSelectedButton = 0;
    flashSelectedButtonTimer = 0;
    flashTargetTime = 1000;
    queuedState = GameState::MemoryMatching;
    nextState = GameState::None;
}

void MainMenuState::updateState()
{
    flashSelectedButtonTimer += 16;

    if (flashSelectedButtonTimer > flashTargetTime)
    {
        flashSelectedButtonTimer = 0;

        if (Output::getLedColor(currentSelectedButton) == Colors::azure)
        {
            flashTargetTime = 1000;
            Output::setLedColor(currentSelectedButton, Colors::yellow);
            Output::setLedIntensity(currentSelectedButton, .5);
        }
        else
        {
            flashTargetTime = 400;
            Output::setLedColor(currentSelectedButton, Colors::azure);
            Output::setLedIntensity(currentSelectedButton, .05);
        }
    }
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 0:
        queuedState = GameState::MemoryMatching;
        break;
    case 12:

        return;
    case 15:
        nextState = queuedState;
        return;
    }

    if (Output::getLedStatus(buttonIndex) && currentSelectedButton != buttonIndex)
    {
        // Change the previously selected button's color to azure with an intensity of 0.1.
        Output::setLedColor(currentSelectedButton, Colors::azure);
        Output::setLedIntensity(currentSelectedButton, .1);

        // Change the newly selected button's color to orange with an intensity of .5.
        Output::setLedColor(buttonIndex, Colors::yellow);
        Output::setLedIntensity(buttonIndex, .5);

        // Play audio tone
        AudioSource::playNote(MusicNote::G5, 100);

        // Reset our flashing variables.
        currentSelectedButton = buttonIndex;
        flashSelectedButtonTimer = 0;
        flashTargetTime = 1000;
    }
}