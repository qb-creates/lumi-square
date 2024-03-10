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
    int16_t countdownTimer;
    int8_t countdownCounter;
    bool isStartingGame;
    void queueGameState(GameState gamestate, Difficulty maxDifficulty, const char *gameStateName);
    void highlightSelectedButton(int8_t selectedButtonIndex);
    void increaseDifficulty();
    void resetDifficulty();
    void displayHighScore();
    void startCountdownTimer();
    void updateHighlightedButtonTimer();
    void updateCountdownTimer();
};

#endif