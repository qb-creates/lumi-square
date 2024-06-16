#include "mainmenustate.h"
#include "buttons.h"
#include "lcd.h"
#include "leds.h"
#include <random.h>

MainMenuState::MainMenuState()
    : GameBaseState(GameState::Menu),
      queuedState(GameState::None),
      maxDifficulty(Difficulty::Easy),
      countdownTimer(1000),
      countdownCounter(3),
      isStartingGame(false),
      gameIndex(0) {}

void MainMenuState::enterState()
{
    LCD::Instance().clearDisplay();
    queueGameState(gameIndex);
}

void MainMenuState::exitState()
{
    countdownTimer = 1000;
    countdownCounter = 3;
    isStartingGame = false;
    nextState = GameState::None;
}

void MainMenuState::updateState()
{
    if (!isStartingGame)
    {
        if (Input::getDifficultyButtonUp() && !AudioSource::Instance().isMuteButtonsPressed())
        {
            increaseDifficulty();
        }

        if (Input::getPreviousButtonDown())
        {
            --gameIndex;

            if (gameIndex < 0)
            {
                gameIndex = 2;
            }

            queueGameState(gameIndex);
        }

        if (Input::getNextButtonDown())
        {
            ++gameIndex;

            if (gameIndex > 2)
            {
                gameIndex = 0;
            }

            queueGameState(gameIndex);
        }
    }

    updateCountdownTimer();
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    if (!isStartingGame)
    {
        if (queuedState == GameState::Simon)
        {
            Output::enableSimonLights(GameProperties::Instance().gameDifficulty != Difficulty::Easy);
        }

        startCountdownTimer();
    }
}

void MainMenuState::queueGameState(int8_t gameIndex)
{
    switch (gameIndex)
    {
    case 0:
        queuedState = GameState::MemoryMatching;
        this->maxDifficulty = Difficulty::Easy;
        LCD::Instance().writeString(0, 3, "Matching   ");
        break;
    case 1:
        queuedState = GameState::Simon;
        this->maxDifficulty = Difficulty::Hard;
        LCD::Instance().writeString(0, 3, "Simon Says ");
        break;
    case 2:
        queuedState = GameState::LightDash;
        this->maxDifficulty = Difficulty::Hard;
        LCD::Instance().writeString(0, 3, "Light Dash ");
        break;
    }

    LCD::Instance().writeString(0, 0, "\x04\x07"); // Write Easy difficulty to lcd
    GameProperties::Instance().setDifficulty(Difficulty::Easy);
    displayHighScore();
}

void MainMenuState::increaseDifficulty()
{
    switch (GameProperties::Instance().increaseDifficulty())
    {
    case Difficulty::Easy:
        LCD::Instance().writeString(0, 0, "\x04\x07"); // Write Easy difficulty to lcd
        break;
    case Difficulty::Medium:
        LCD::Instance().writeString(0, 0, "\x05\x07"); // Write Medium difficulty to lcd
        break;
    case Difficulty::Hard:
        LCD::Instance().writeString(0, 0, "\x06\x07"); // Write Hard difficulty to lcd
        break;
    }

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

    // Show countdown text
    LCD::Instance().writeString(0, 0, "   Starting.. ");
    LCD::Instance().writeString(1, 0, "       3        ");

    AudioSource::Instance().playMusicNote(MusicNote::C5, 200);
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
