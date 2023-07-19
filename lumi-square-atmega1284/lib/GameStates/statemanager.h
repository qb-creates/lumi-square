#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "basestate.h"
#include "MainMenu/mainmenustate.h"
#include "MemoryMatching/matchingstate.h"

class StateManager
{
public:
    StateManager(const StateManager &) = delete;
    void operator=(const StateManager &) = delete;
    static StateManager &Instance();
    void update();
    void buttonPressed();

private:
    StateManager();
    GameBaseState *currentState;
    MainMenuState mainMenuState;
    MemoryMatchingState memoryMatchingState;
    GameBaseState *stateDictionary[4];
};

#endif