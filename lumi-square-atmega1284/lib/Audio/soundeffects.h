#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H

#include "audioclip.h"
#include <stdint.h>

const MusicNote POWER_UP_SOUND_EFFECT_DATA[] = {
    MusicNote::C4, MusicNote::Eighth,
    MusicNote::E4, MusicNote::Eighth,
    MusicNote::G4, MusicNote::Eighth,
    MusicNote::C5, MusicNote::Eighth,
    MusicNote::E5, MusicNote::Eighth,
    MusicNote::END
};

const MusicNote FAILURE_SOUND_EFFECT_DATA[] = {
    MusicNote::D3, MusicNote::Eighth,
    MusicNote::B2, MusicNote::Quarter,
    MusicNote::END
};

const AudioClip FAILURE_AUDIO_CLIP(FAILURE_SOUND_EFFECT_DATA, 130);
const AudioClip POWER_UP_AUDIO_CLIP(POWER_UP_SOUND_EFFECT_DATA, 240);

#endif