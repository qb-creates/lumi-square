#include "gameoverstate.h"
#include "ledmatrix.h"

GameOverState::GameOverState()
    : GameBaseState(GameState::GameOver),
      flashAnimationTimer(200),
      transitionToMainTimer(2000),
      flashCount(7),
      onLED{false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false} {}

void GameOverState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);

    for (int i = 0; i < 16; i++)
    {
        onLED[i] = LEDMatrix::getLedStatus(i);
    }
}

void GameOverState::exitState()
{
    for (int i = 0; i < 16; i++)
    {
        LEDMatrix::ledOff(i);
        onLED[i] = false;
    }

    flashAnimationTimer.setTargetTime(200);
    transitionToMainTimer.setTargetTime(2000);
    flashCount = 7;
    nextState = GameState::None;
}

void GameOverState::updateState()
{
    if (transitionToMainTimer.isComplete() && AudioSource::Instance().isVoiceOverPlaying())
        return;

    if (transitionToMainTimer.isComplete() && !AudioSource::Instance().isVoiceOverPlaying())
    {
        nextState = GameState::Menu;
        return;
    }

    if (!flashAnimationTimer.isComplete())
    {
        flashAnimationTimer.updateTimer(DeviceUtility::DELTA_TIME);

        if (flashAnimationTimer.isComplete())
        {
            for (int i = 0; i < 16; i++)
            {
                if (flashCount % 2)
                {
                    if (onLED[i])
                    {
                        LEDMatrix::ledOff(i);
                    }
                }
                else
                {
                    if (onLED[i])
                    {
                        LEDMatrix::ledOn(i);
                    }
                }
            }

            if (flashCount != 0)
            {
                flashCount--;
                flashAnimationTimer.setTargetTime(300);
            }
        }

        return;
    }

    if (!transitionToMainTimer.isComplete())
    {
        transitionToMainTimer.updateTimer(DeviceUtility::DELTA_TIME);

        if (transitionToMainTimer.isComplete())
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
