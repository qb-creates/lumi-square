#include "mainmenustate.h"
#include "audio.h"
#include "leds.h"

MainMenuState::MainMenuState() : GameBaseState(), queuedState(GameState::None), previouslySelectedButton(0) {}

void MainMenuState::enterState()
{
    for (int i = 1; i < 8; i++)
    {
        Output::setLedColor(i, Colors::aquamarine);
    }

    Output::setLedColor(0, Colors::orange);
    Output::setLedColor(15, Colors::cyan);
    Output::setLedColor(12, Colors::yellow);
    queuedState = GameState::MemoryMatching;

}

void MainMenuState::exitState()
{
    queuedState = GameState::None;
    nextState = GameState::None;
}

void MainMenuState::updateState() {}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 0:
        queuedState = GameState::MemoryMatching;
        break;
    case 12:

        break;
    case 15:
        nextState = queuedState;
        break;
    }

    if (previouslySelectedButton != buttonIndex)
    {
        Output::setLedColor(previouslySelectedButton, Colors::aquamarine);
        Output::setLedColor(buttonIndex, Colors::orange);
        AudioSource::playNote(MusicNote::C6, 100);
        previouslySelectedButton = buttonIndex;
    }
}