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
    int8_t sequenceIndex;
    int8_t activeLedIndex;
    int8_t currentRound;
    int8_t simonLedSequence[100];
    MusicNote simonMusicNoteSequence[100];
    int8_t ledOptions[8];
    MusicNote musicNoteOptions[8];
    bool listeningForPlayerInput;
    int16_t ledBrightnessAdjustTimer;
    int8_t lives;
    void playRoundSequence();
    void listenForPlayerInput();
    void playNextSequenceNote();
    void incrementRound();
};

#endif