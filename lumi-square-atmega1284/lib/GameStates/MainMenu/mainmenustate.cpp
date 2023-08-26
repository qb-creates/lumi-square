#include "mainmenustate.h"
#include "audio.h"
#include "lcd.h"
#include "leds.h"

MainMenuState::MainMenuState()
    : GameBaseState(), queuedState(GameState::MemoryMatching), currentSelectedButton(0), flashSelectedButtonTimer(0), flashTargetTime(800) {}

void MainMenuState::enterState()
{
    LCD::Instance().clearDisplay();
    LCD::Instance().writeChars(0, 0, "Memory Matching ");
    LCD::Instance().writeChars(1, 0, "High Score:");

    for (int i = 1; i < 8; i++)
    {
        Output::ledOn(i, Colors::azure, .05);
    }

    setDifficultyLightColor();

    Output::ledOn(0, Colors::yellow, .5);
    Output::ledOn(15, Colors::aquamarine, 1);
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
            Output::setLedColor(currentSelectedButton, Colors::yellow, .5);
        }
        else
        {
            flashTargetTime = 400;
            Output::setLedColor(currentSelectedButton, Colors::azure, .05);
        }
    }
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 0:
        LCD::Instance().writeChars(0, 0, "Memory Matching ");
        queuedState = GameState::MemoryMatching;
        break;
    case 1:
        LCD::Instance().writeChars(0, 0, "Simon           ");
        queuedState = GameState::Simon;
        break;
    case 2:
        LCD::Instance().writeChars(0, 0, "Light Speed     ");
        queuedState = GameState::LightSpeed;
        break;
    case 12:
        GameProperties::Instance().increaseDifficulty();
        setDifficultyLightColor();
        return;
    case 15:
        nextState = queuedState;
        return;
    }

    if (Output::getLedStatus(buttonIndex) && currentSelectedButton != buttonIndex)
    {
        // Change the previously selected button's color to azure with an intensity of 0.1.
        Output::setLedColor(currentSelectedButton, Colors::azure, .1);

        // Change the newly selected button's color to orange with an intensity of .5.
        Output::setLedColor(buttonIndex, Colors::yellow, .5);

        // Play audio tone
        AudioSource::playNote(MusicNote::G5, 100);

        // Reset our flashing variables.
        currentSelectedButton = buttonIndex;
        flashSelectedButtonTimer = 0;
        flashTargetTime = 1000;
    }
}

void MainMenuState::setDifficultyLightColor()
{
    switch (GameProperties::Instance().gameDifficulty)
    {
    case Difficulty::Easy:
        Output::ledOn(12, Colors::green, 0.5);
        break;
    case Difficulty::Medium:
        Output::ledOn(12, Colors::orange, 0.5);
        break;
    case Difficulty::Hard:
        Output::ledOn(12, Colors::red, 0.5);
        break;
    }
}
