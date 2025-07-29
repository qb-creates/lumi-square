#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "basestate.h"

class MainMenuState : public GameBaseState
{
public:
    MainMenuState();
    void enterState(GameState previousState);
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
    bool firstEntry;
    void queueGameState(int8_t gameIndex, bool playAudio);
    void increaseDifficulty();
    void resetDifficulty();
    void startCountdownTimer();
    void updateCountdownTimer();
};

#endif