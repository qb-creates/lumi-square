#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "basestate.h"

class GameOverState : public GameBaseState
{
public:
    GameOverState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    GameState queuedState;
    int16_t flashButtonTimer;
    int16_t flashTargetTime;
    int16_t transitionTimer;
    bool flip;
};

#endif