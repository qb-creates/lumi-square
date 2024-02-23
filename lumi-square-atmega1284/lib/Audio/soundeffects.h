#ifndef SOUNDEFFECTS_H
#define SOUNDEFFECTS_H

#include "audioclip.h"
#include <stdint.h>

const int16_t tetrisTheme[] = { 
  static_cast<int16_t>(MusicNote::E4), 4,  static_cast<int16_t>(MusicNote::B3),8,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::D4),4,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::B3),8,
  static_cast<int16_t>(MusicNote::A3), 4,  static_cast<int16_t>(MusicNote::A3),8,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::E4),4,  static_cast<int16_t>(MusicNote::D4),8,  static_cast<int16_t>(MusicNote::C4),8,
  static_cast<int16_t>(MusicNote::B3), -4,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::D4),4,  static_cast<int16_t>(MusicNote::E4),4,
  static_cast<int16_t>(MusicNote::C4), 4,  static_cast<int16_t>(MusicNote::A3),4,  static_cast<int16_t>(MusicNote::A3),8,  static_cast<int16_t>(MusicNote::A3),4,  static_cast<int16_t>(MusicNote::B3),8,  static_cast<int16_t>(MusicNote::C4),8,

  static_cast<int16_t>(MusicNote::D5), -4,  static_cast<int16_t>(MusicNote::F5),8,  static_cast<int16_t>(MusicNote::A5),4,  static_cast<int16_t>(MusicNote::G5),8,  static_cast<int16_t>(MusicNote::F5),8,
  static_cast<int16_t>(MusicNote::E5), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),4, static_cast<int16_t>(MusicNote::Rest), 4,

  static_cast<int16_t>(MusicNote::E5), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::B4),8,
  static_cast<int16_t>(MusicNote::A4), 4,  static_cast<int16_t>(MusicNote::A4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),8,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,

  static_cast<int16_t>(MusicNote::D5), -4,  static_cast<int16_t>(MusicNote::F5),8,  static_cast<int16_t>(MusicNote::A5),4,  static_cast<int16_t>(MusicNote::G5),8,  static_cast<int16_t>(MusicNote::F5),8,
  static_cast<int16_t>(MusicNote::E5), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),4, static_cast<int16_t>(MusicNote::Rest), 4,
  

  static_cast<int16_t>(MusicNote::E5),2,  static_cast<int16_t>(MusicNote::C5),2,
  static_cast<int16_t>(MusicNote::D5),2,   static_cast<int16_t>(MusicNote::B4),2,
  static_cast<int16_t>(MusicNote::C5),2,   static_cast<int16_t>(MusicNote::A4),2,
  static_cast<int16_t>(MusicNote::Ab4),2,  static_cast<int16_t>(MusicNote::B4),4,  static_cast<int16_t>(MusicNote::Rest),8, 
  static_cast<int16_t>(MusicNote::E5),2,   static_cast<int16_t>(MusicNote::C5),2,
  static_cast<int16_t>(MusicNote::D5),2,   static_cast<int16_t>(MusicNote::B4),2,
  static_cast<int16_t>(MusicNote::C5),4,   static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::A5),2,
  static_cast<int16_t>(MusicNote::Ab4),2,
};

const MusicNote FAILURE_SOUND_EFFECT_DATA[] = {
    MusicNote::D3, MusicNote::Eighth,
    MusicNote::B2, MusicNote::Quarter,
    MusicNote::END
};

const AudioClip FAILURE_AUDIO_CLIP(FAILURE_SOUND_EFFECT_DATA, 130);

#endif