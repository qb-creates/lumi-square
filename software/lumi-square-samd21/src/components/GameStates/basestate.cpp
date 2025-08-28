#include "basestate.h"

GameBaseState::GameBaseState(GameState gameState)
    : currentState(gameState),
      previousState(GameState::None),
      nextState(GameState::None) {}

GameState GameBaseState::switchStateCheck()
{
    return nextState;
}

void GameBaseState::enterState(GameState previousState)
{
    this->previousState = previousState;
}