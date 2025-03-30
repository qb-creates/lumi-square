#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "basestate.h"

class GameOverState : public GameBaseState
{
public:
    GameOverState();
    void enterState(GameState previousState);
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    int16_t flashAnimationTimer;
    int16_t transitionToMainTimer;
    int8_t flashCount;
    bool onLED[16];
};

#endif