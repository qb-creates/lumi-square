#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "basestate.h"
#include "timer.h"
class GameOverState : public GameBaseState
{
public:
    GameOverState();
    void enterState(GameState previousState);
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    Timer flashAnimationTimer;
    Timer transitionToMainTimer;
    int8_t flashCount;
    bool onLED[16];
};

#endif