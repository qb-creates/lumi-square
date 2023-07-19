#include "basestate.h"

GameBaseState::GameBaseState() : nextState(GameState::None) {}


GameState GameBaseState::switchStateCheck()
{
    return nextState;
}