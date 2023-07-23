#include "gameoverstate.h"
#include "leds.h"

GameOverState::GameOverState()
    : GameBaseState(), queuedState(GameState::None), flashButtonTimer(0), flashTargetTime(2000), transitionTimer(0), flip(true) {}

void GameOverState::enterState()
{
    Output::ledOn(12);
    Output::setLedIntensity(12, 1);
    Output::setLedColor(12, Colors::orange);

    Output::ledOn(15);
    Output::setLedIntensity(15, 1);
    Output::setLedColor(15, Colors::green);
}

void GameOverState::exitState()
{
    for (int i = 0; i < 16; i++)
    {
        Output::ledOff(i);
    }

    flip = true;
    flashButtonTimer = 0;
    flashTargetTime = 2000;
    transitionTimer = 0;
    queuedState = GameState::None;
    nextState = GameState::None;
}

void GameOverState::updateState()
{
    flashButtonTimer += 16;

    if (flashButtonTimer > flashTargetTime && transitionTimer == 0)
    {
        flashButtonTimer = 0;
        flip = !flip;

        if (flip)
        {
            Output::ledOn(12);
            Output::ledOn(15);
            flashTargetTime = 2000;
        }
        else
        {
            Output::ledOff(12);
            Output::ledOff(15);
            flashTargetTime = 400;
        }
    }

    if (transitionTimer > 0)
    {
        transitionTimer -= 16;

        if (transitionTimer <= 0)
        {
            nextState = queuedState;
        }
    }
}

void GameOverState::onButtonPressed(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 12:
        queuedState = GameState::Previous;
        transitionTimer = 400;
        break;
    case 15:
        queuedState = GameState::Menu;
        transitionTimer = 400;
        break;
    }
}
