#ifndef GAMEBASESTATE_H
#define GAMEBASESTATE_H

#include <stdint.h>
#include "soundeffects.h"
#include "audio.h"
#include "gameproperties.h"
#include "highscoremanager.h"

class GameBaseState
{
public:
    GameBaseState(GameState currentState);
    GameState switchStateCheck();
    virtual void enterState() = 0;
    virtual void exitState() = 0;
    virtual void updateState() = 0;
    virtual void onButtonPressed(int8_t buttonIndex) = 0;

protected:
    const GameState currentState;
    GameState nextState;
};

#endif