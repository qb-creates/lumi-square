#include "statemanager.h"
#include "buttons.h"
#include "fixedupdate.h"

StateManager::StateManager()
    : FixedUpdateEventListener(), currentState(), gameOverState(), mainMenuState(), memoryMatchingState(), lightSpeedState(), simonState(), stateDictionary{}
{
    stateDictionary[1] = &gameOverState;
    stateDictionary[2] = &mainMenuState;
    stateDictionary[3] = &memoryMatchingState;
    stateDictionary[5] = &lightSpeedState;
    stateDictionary[6] = &simonState;
    currentState = &mainMenuState;
    currentState->enterState();
}

StateManager &StateManager::Instance()
{
    static StateManager instance;
    return instance;
}

void StateManager::onFixedUpdate()
{
    buttonPressed();
    update();
}

void StateManager::update()
{
    currentState->updateState();
    GameState stateKey = currentState->switchStateCheck();

    if (stateKey != GameState::None)
    {
        GameBaseState *nextState = stateDictionary[static_cast<int8_t>(stateKey)];

        if (currentState != nextState)
        {
            currentState->exitState();
            currentState = nextState;
            currentState->enterState();
        }
    }
}

void StateManager::buttonPressed()
{
    for (int i = 0; i < 16; i++)
    {
        if (Input::getButtonDown(i))
        {
            currentState->onButtonPressed(i);
        }
    }
}
