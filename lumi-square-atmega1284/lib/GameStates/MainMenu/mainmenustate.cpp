#include "mainmenustate.h"
#include "audio.h"
#include "lcd.h"
#include "leds.h"
#include <random.h>

MainMenuState::MainMenuState()
    : GameBaseState(),
      queuedState(GameState::MemoryMatching),
      maxDifficulty(Difficulty::Easy),
      highlightedButtonIndex(0),
      highlightedButtonTimer(1000) {}

void MainMenuState::enterState()
{
    uint8_t gameSelectButtons[] = {1, 2, 3, 4, 5, 6, 7};
    Output::ledOn(gameSelectButtons, 7, Colors::azure, .05);

    Output::ledOn(0, Colors::yellow, .6);
    Output::ledOn(15, Colors::yellow, .4);
    resetDifficulty();
    queueGameState(GameState::MemoryMatching, Difficulty::Easy, "Memory Matching ");
}

void MainMenuState::exitState()
{
    uint8_t allMenuButtons[] = {0, 1, 2, 3, 4, 5, 6, 7, 12, 15};
    Output::ledOff(allMenuButtons, 10, Colors::azure, .05);

    highlightedButtonIndex = 0;
    highlightedButtonTimer = 1000;
    nextState = GameState::None;
}

void MainMenuState::updateState()
{
    highlightedButtonTimer -= 16;

    if (highlightedButtonTimer > 0)
        return;

    if (Output::getLedColor(highlightedButtonIndex) == Colors::azure)
    {
        highlightedButtonTimer = 1000;
        Output::setLedColor(highlightedButtonIndex, Colors::yellow, .5);
    }
    else
    {
        highlightedButtonTimer = 400;
        Output::setLedColor(highlightedButtonIndex, Colors::azure, .05);
    }
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 0:
        queueGameState(GameState::MemoryMatching, Difficulty::Easy, "Memory Matching ");
        break;
    case 1:
        queueGameState(GameState::Simon, Difficulty::Hard, "Simon           ");
        break;
    case 2:
        queueGameState(GameState::LightSpeed, Difficulty::Hard, "Light Speed     ");
        break;
    case 12:
        increaseDifficulty();
        return;
    case 15:
        Random::seedRNG();
        nextState = queuedState;
        return;
    }

    resetDifficulty();
    highlightSelectedButton(buttonIndex);
}

void MainMenuState::queueGameState(GameState gamestate, Difficulty maxDifficulty, const char *gameStateName)
{
    this->maxDifficulty = maxDifficulty;
    queuedState = gamestate;
    LCD::Instance().writeString(0, 0, gameStateName);
    LCD::Instance().writeString(1, 0, "High Score:     ");
}

void MainMenuState::highlightSelectedButton(int8_t selectedButtonIndex)
{
    if (Output::getLedStatus(selectedButtonIndex) && highlightedButtonIndex != selectedButtonIndex)
    {
        Output::setLedColor(highlightedButtonIndex, Colors::azure, .05);
        Output::setLedColor(selectedButtonIndex, Colors::yellow, .5);
        AudioSource::playNote(MusicNote::G4, 100);
        highlightedButtonIndex = selectedButtonIndex;
        highlightedButtonTimer = 1000;
    }
}

void MainMenuState::increaseDifficulty()
{
    if (GameProperties::Instance().gameDifficulty == maxDifficulty)
    {
        resetDifficulty();
        return;
    }

    switch (GameProperties::Instance().increaseDifficulty())
    {
    case Difficulty::Easy:
        Output::ledOn(12, Colors::green, 0.4);
        break;
    case Difficulty::Medium:
        Output::ledOn(12, Colors::orange, 0.4);
        break;
    case Difficulty::Hard:
        Output::ledOn(12, Colors::red, 0.4);
        break;
    }
}

void MainMenuState::resetDifficulty()
{
    Output::ledOn(12, Colors::green, 0.4);
    GameProperties::Instance().setDifficulty(Difficulty::Easy);
}