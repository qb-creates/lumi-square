#ifndef VOICEOVERMANAGER_H
#define VOICEOVERMANAGER_H

#include "stdint.h"
#include "voiceover.h"

class VoiceOverManager
{
public:
    VoiceOverManager(const VoiceOverManager &) = delete;
    void operator=(const VoiceOverManager &) = delete;
    static void Update();
    static bool IsVoiceOverPlaying();
    static void PlayVoiceOver(VoiceOver voiceOver);
    static void QueueNumberVoiceOver(uint16_t number);
    static void QueueVoiceOver(VoiceOver voiceOver);    
};

#endif