#include "gameoverstate.h"
#include "leds.h"

GameOverState::GameOverState()
    : GameBaseState(GameState::GameOver),
      flashAnimationTimer(200),
      transitionToMainTimer(2000),
      flashCount(7),
      onLED{} {}

void GameOverState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);

    for (int i = 0; i < 16; i++)
    {
        onLED[i] = Output::getLedStatus(i);
    }
}

void GameOverState::exitState()
{
    for (int i = 0; i < 16; i++)
    {
        Output::ledOff(i);
        onLED[i] = false;
    }

    flashAnimationTimer = 200;
    transitionToMainTimer = 2000;
    flashCount = 7;
    nextState = GameState::None;
}

void GameOverState::updateState()
{
    if (transitionToMainTimer <= 0 && AudioSource::Instance().isVoiceOverPlaying())
        return;

    if (transitionToMainTimer <= 0 && !AudioSource::Instance().isVoiceOverPlaying())
    {
        nextState = GameState::Menu;
        return;
    }

    if (flashAnimationTimer > 0)
    {
        flashAnimationTimer -= FixedUpdateTimer::DELTA_TIME;

        if (flashAnimationTimer <= 0)
        {
            for (int i = 0; i < 16; i++)
            {
                if (flashCount % 2)
                {
                    if (onLED[i])
                    {
                        Output::ledOff(i);
                    }
                }
                else
                {
                    if (onLED[i])
                    {
                        Output::ledOn(i);
                    }
                }
            }

            if (flashCount != 0)
            {
                flashCount--;
                flashAnimationTimer = 300;
            }
        }

        return;
    }

    if (transitionToMainTimer > 0)
    {
        transitionToMainTimer -= FixedUpdateTimer::DELTA_TIME;

        if (transitionToMainTimer <= 0)
        {
            if (previousState == GameState::MemoryMatching)
            {
                nextState = GameState::Menu;
                return;
            }

            if (ScoreManager::Instance().newHighScoreAchieved())
                AudioSource::Instance().queueVoiceOver(DFPlayerCommand::NewHighScore);

            uint8_t score = ScoreManager::Instance().getScore();
            AudioSource::Instance().queueVoiceOver(DFPlayerCommand::Score);
            AudioSource::Instance().queueNumberVoiceOver(score);
        }
    }
}

void GameOverState::onButtonPressed(int8_t buttonIndex) {}
