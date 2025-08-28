#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "GameOver/gameoverstate.h"
#include "LightDash/lightdashstate.h"
#include "MainMenu/mainmenustate.h"
#include "MemoryMatching/matchingstate.h"
#include "Simon/simonstate.h"
#include "PowerOn/poweronstate.h"
#include "basestate.h"
#include "fixedupdate.h"

class StateManager : public FixedUpdateEventListener
{
public:
    static StateManager &Instance();
    void onFixedUpdate() override;

private:
    StateManager();
    StateManager(const StateManager &) = delete;
    void operator=(const StateManager &) = delete;
    void update();
    void buttonPressed();

private:
    GameBaseState *currentState;
    PowerOnState powerOnState;
    GameOverState gameOverState;
    MainMenuState mainMenuState;
    MemoryMatchingState memoryMatchingState;
    LightDashState lightSpeedState;
    SimonState simonState;
    GameBaseState *stateDictionary[10];

};

#endif