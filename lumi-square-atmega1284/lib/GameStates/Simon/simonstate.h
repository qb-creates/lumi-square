#ifndef SIMON_H
#define SIMON_H

#include "basestate.h"
#include "audio.h"

class SimonState : public GameBaseState
{
public:
    SimonState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    void playNewSequence();
    int8_t pattern[100];
    MusicNote patternNotes[100];
    int8_t patternIndex;
    int8_t playPattern;
    bool isPlayingSequence;
    int16_t timer;
    bool buttonLedIsOn;
    bool gameOver;
};

#endif