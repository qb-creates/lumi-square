#include "basestate.h"

GameBaseState::GameBaseState(GameState gameState) : currentState(gameState), nextState(GameState::None) {}

GameState GameBaseState::switchStateCheck()
{
    return nextState;
}
