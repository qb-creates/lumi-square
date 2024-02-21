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
    Difficulty maxDifficulty;
    int8_t highlightedButtonIndex;
    int16_t highlightedButtonTimer;
    double test;
    void queueGameState(GameState gamestate, Difficulty maxDifficulty, const char *gameStateName);
    void highlightSelectedButton(int8_t selectedButtonIndex);
    void increaseDifficulty();
    void resetDifficulty();
    void displayHighScore();
};

#endif