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
    const uint8_t buttonMapArray[8];
    const MusicNote musicNoteMapArray[8];
    int8_t sequenceIndex;
    int8_t activeButtonIndex;
    int8_t currentRound;
    int8_t simonButtonSequence[100];
    MusicNote simonMusicNoteSequence[100];
    bool listeningForPlayerInput;
    int16_t delayTimer;
    int8_t lives;
    uint8_t *highScoreAddress;
    void startNextRoundSequence();
    void replayRoundSequence();
    void listenForPlayerInput();
    void playNextSequenceElement();
    void removeLifePoint();
    void saveHighScore();
};

#endif