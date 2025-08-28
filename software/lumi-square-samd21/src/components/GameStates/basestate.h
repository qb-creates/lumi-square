#ifndef GAMEBASESTATE_H
#define GAMEBASESTATE_H

#include "audio.h"
#include "gameproperties.h"
#include "scoremanager.h"
#include <stdint.h>

class GameBaseState
{
public:
    GameBaseState(GameState currentState);
    GameState switchStateCheck();
    virtual void enterState(GameState previousState);
    virtual void exitState() = 0;
    virtual void updateState() = 0;
    virtual void onButtonPressed(int8_t buttonIndex) = 0;

public:
    const GameState currentState;

protected:
    GameState previousState;
    GameState nextState;
};

#endif