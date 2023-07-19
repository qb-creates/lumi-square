#ifndef GAMEBASESTATE_H
#define GAMEBASESTATE_H

#include <stdint.h>

enum class GameState
{
    None = -1,
    Previous = 0,
    GameOver = 1,
    Menu = 2,
    MemoryMatching = 3,
    TicTacToe = 4,
    LightSpeed = 5,
    Simon = 6
};

class GameBaseState
{
public:
    GameBaseState();
    GameState switchStateCheck();
    virtual void enterState() = 0;
    virtual void exitState() = 0;
    virtual void updateState() = 0;
    virtual void onButtonPressed(int8_t buttonIndex) = 0;

protected:
    GameState nextState;
};

#endif

