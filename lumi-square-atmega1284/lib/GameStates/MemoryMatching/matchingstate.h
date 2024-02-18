#ifndef MATCHINGSTATE_H
#define MATCHINGSTATE_H

#include "basestate.h"
#include "colors.h"

class MemoryMatchingState : public GameBaseState
{
public:
    MemoryMatchingState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    static Color colorList[16];
    int16_t ledBrightnessAdjustTimer;
    int8_t correctMatches;
    int8_t selectedLedIndex1;
    int8_t selectedLedIndex2;
    bool isGuessCorrect;
    void shuffleLedColors();
    void evaluateGuess();
    void adjustLedBrightness();
};

#endif