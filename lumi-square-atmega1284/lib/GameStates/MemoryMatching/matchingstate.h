#ifndef MATCHINGSTATE_H
#define MATCHINGSTATE_H

#include "basestate.h"

class MemoryMatchingState : public GameBaseState
{
public:
    MemoryMatchingState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    void randomizeLights();
    int8_t firstGuessIndex;
    int8_t secondGuessIndex;
    bool guessCorrect;
    int16_t timer;
    int8_t correctGueses;
};

#endif