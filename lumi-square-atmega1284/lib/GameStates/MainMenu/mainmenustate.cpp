#include "mainmenustate.h"
#include "lcd.h"
#include "leds.h"
#include <random.h>

MainMenuState::MainMenuState()
    : GameBaseState(GameState::Menu),
      queuedState(GameState::None),
      maxDifficulty(Difficulty::Easy),
      highlightedButtonIndex(0),
      highlightedButtonTimer(1000),
      countdownTimer(1000),
      countdownCounter(3),
      isStartingGame(false) {}

void MainMenuState::enterState()
{
    // Set the color for all game select buttons to blue.
    uint8_t gameSelectButtons[] = {1, 2, 3, 4, 5, 6, 7};
    Output::ledOn(gameSelectButtons, 7, Colors::azure, .5);

    // Set the color for the first game select button to yellow.
    Output::ledOn(0, Colors::yellow, 1);

    // Set the color for the start game button to yellow.
    Output::ledOn(15, Colors::yellow, .8);

    LCD::Instance().clearDisplay();
    queueGameState(GameState::MemoryMatching, Difficulty::Easy, "Matching   ");
    resetDifficulty();
}

void MainMenuState::exitState()
{
    countdownTimer = 1000;
    countdownCounter = 3;
    highlightedButtonIndex = 0;
    highlightedButtonTimer = 1000;
    isStartingGame = false;
    nextState = GameState::None;
    queuedState = GameState::None;
}

void MainMenuState::updateState()
{
    updateHighlightedButtonTimer();
    updateCountdownTimer();
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 0:
        queueGameState(GameState::MemoryMatching, Difficulty::Easy, "Matching   ");
        break;
    case 1:
        queueGameState(GameState::Simon, Difficulty::Hard, "Simon Says ");
        break;
    case 2:
        queueGameState(GameState::LightDash, Difficulty::Hard, "Light Dash ");
        break;
    case 12:
        increaseDifficulty();
        return;
    case 15:
        startCountdownTimer();
        return;
    }

    highlightSelectedButton(buttonIndex);
}

void MainMenuState::queueGameState(GameState gamestate, Difficulty maxDifficulty, const char *gameStateName)
{
    if (queuedState == gamestate)
        return;

    this->maxDifficulty = maxDifficulty;
    queuedState = gamestate;
    LCD::Instance().writeString(0, 3, gameStateName);
    resetDifficulty();
}

void MainMenuState::highlightSelectedButton(int8_t selectedButtonIndex)
{
    if (Output::getLedStatus(selectedButtonIndex) && highlightedButtonIndex != selectedButtonIndex)
    {
        Output::setLedColor(highlightedButtonIndex, Colors::azure, .5);
        Output::setLedColor(selectedButtonIndex, Colors::yellow, 1);
        AudioSource::Instance().playMusicNote(MusicNote::G4, 100);
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
        Output::ledOn(12, Colors::green, 1);
        LCD::Instance().writeString(0, 0, "\x04\x07");
        break;
    case Difficulty::Medium:
        LCD::Instance().writeString(0, 0, "\x05\x07");
        Output::ledOn(12, Colors::orange, 1);
        break;
    case Difficulty::Hard:
        Output::ledOn(12, Colors::red, 1);
        LCD::Instance().writeString(0, 0, "\x06\x07");
        break;
    }

    displayHighScore();
}

void MainMenuState::resetDifficulty()
{
    Output::ledOn(12, Colors::green, 0.4);
    LCD::Instance().writeString(0, 0, "\x04\x07");
    GameProperties::Instance().setDifficulty(Difficulty::Easy);
    displayHighScore();
}

void MainMenuState::displayHighScore()
{
    int8_t savedHighScore = HighScoreManager::getHighScore(queuedState, GameProperties::Instance().gameDifficulty);
    LCD::Instance().writeString(1, 0, "   HiScr:");
    LCD::Instance().writeNumber(1, 10, savedHighScore, true);
}

void MainMenuState::startCountdownTimer()
{
    isStartingGame = true;

    // Power off all all menu buttons
    uint8_t allMenuButtons[] = {0, 1, 2, 3, 4, 5, 6, 7, 12, 15};
    Output::ledOff(allMenuButtons, 10, Colors::azure, .5);

    // Show countdown text
    LCD::Instance().writeString(0, 0, "   Starting.. ");
    LCD::Instance().writeString(1, 0, "       3        ");

    AudioSource::Instance().playMusicNote(MusicNote::C5, 200);
}

void MainMenuState::updateHighlightedButtonTimer()
{
    highlightedButtonTimer -= 16;

    if (highlightedButtonTimer <= 0)
    {
        if (Output::getLedColor(highlightedButtonIndex) == Colors::azure)
        {
            highlightedButtonTimer = 1000;
            Output::setLedColor(highlightedButtonIndex, Colors::yellow, 1);
        }
        else
        {
            highlightedButtonTimer = 400;
            Output::setLedColor(highlightedButtonIndex, Colors::azure, .5);
        }
    }
}

void MainMenuState::updateCountdownTimer()
{
    countdownTimer -= isStartingGame ? 16 : 0;

    if (countdownTimer <= 0)
    {
        --countdownCounter;
        countdownTimer = 1000;

        if (countdownCounter < 0)
        {
            Random::seedRNG();
            nextState = queuedState;
            return;
        }

        LCD::Instance().writeNumber(1, 7, countdownCounter, true);
        AudioSource::Instance().playMusicNote(countdownCounter == 0 ? MusicNote::E5 : MusicNote::C5, 200);
    }
}
