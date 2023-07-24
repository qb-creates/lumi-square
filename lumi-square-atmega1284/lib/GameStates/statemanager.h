#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "basestate.h"
#include "MainMenu/mainmenustate.h"
#include "MemoryMatching/matchingstate.h"
#include "GameOver/gameoverstate.h"
#include "Simon/simonstate.h"

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
    GameOverState gameOverState;
    MainMenuState mainMenuState;
    MemoryMatchingState memoryMatchingState;
    SimonState simonState;
    GameBaseState *stateDictionary[10];
};

#endif