#ifndef AUDIOCLIP_H
#define AUDIOCLIP_H

#include "musicnotes.h"
#include <stdint.h>

class AudioClip
{
public:
    const MusicNote *audioData;
    uint8_t bpm;
    AudioClip(const MusicNote *audioData, uint8_t bpm);
    void operator=(const AudioClip &) = delete;
};

#endif