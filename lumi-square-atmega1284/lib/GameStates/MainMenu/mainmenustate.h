#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "basestate.h"

class MainMenuState : public GameBaseState
{
public:
    MainMenuState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    GameState queuedState;
    int8_t currentSelectedButton;
    int16_t flashSelectedButtonTimer;
    int16_t flashTargetTime;
    void setDifficultyLEDColor();
};

#endif