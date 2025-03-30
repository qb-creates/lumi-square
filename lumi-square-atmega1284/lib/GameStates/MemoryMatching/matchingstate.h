#ifndef MATCHINGSTATE_H
#define MATCHINGSTATE_H

#include "../basestate.h"
#include "../../Colors/colors.h"

class MemoryMatchingState : public GameBaseState
{
public:
    MemoryMatchingState();
    void enterState(GameState previousState);
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    int16_t ledBrightnessAdjustTimer;
    int8_t correctMatches;
    int8_t selectedLedIndex1;
    int8_t selectedLedIndex2;
    bool isGuessCorrect;
    Color colorList[16];
    void shuffleLedColors();
    void evaluateGuess();
    void adjustLedBrightness();
};

#endif