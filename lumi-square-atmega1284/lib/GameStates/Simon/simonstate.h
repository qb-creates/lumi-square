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
    int8_t simonSequenceIndex;
    int8_t playerSequenceIndex;
    int8_t simonLightSequence[100];
    MusicNote simonMusicNoteSequence[100];
    int8_t buttonOptions[8];
    MusicNote musicNoteOptions[16];
    bool isPlayingSequence;
    int16_t timer;
    bool buttonLedIsOn;
    int lives;
};

#endif