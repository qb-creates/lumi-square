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
    int16_t countdownTimer;
    int8_t countdownCounter;
    bool isStartingGame;
    int8_t gameIndex;
    int8_t previousButtonDelay;
    int8_t nextButtonDelay;
    void queueGameState(int8_t gameIndex);
    void increaseDifficulty();
    void resetDifficulty();
    void displayHighScore();
    void startCountdownTimer();
    void updateCountdownTimer();
};

#endif