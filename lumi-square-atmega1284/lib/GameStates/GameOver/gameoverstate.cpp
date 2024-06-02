#include "gameoverstate.h"
#include "leds.h"

GameOverState::GameOverState()
    : GameBaseState(GameState::GameOver), flashAnimationTimer(200), transitionToMainTimer(0), flashCount(7), onLED{} {}

void GameOverState::enterState()
{
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
    transitionToMainTimer = 0;
    flashCount = 7;
    nextState = GameState::None;
}

void GameOverState::updateState()
{
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
            else
            {
                transitionToMainTimer = 2000;
            }
        }
    }

    if (transitionToMainTimer > 0)
    {
        transitionToMainTimer -= FixedUpdateTimer::DELTA_TIME;

        if (transitionToMainTimer <= 0)
        {
            nextState = GameState::Menu;
        }
    }
}

void GameOverState::onButtonPressed(int8_t buttonIndex) {}
