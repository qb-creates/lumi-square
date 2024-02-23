#include "audioclip.h"

/**
 * @brief Construct a new Audio Clip:: Audio Clip object
 * 
 * @param audioData 
 * @param bpm 
 */
AudioClip::AudioClip(const MusicNote *audioData , uint8_t bpm)
    : audioData(audioData),
      bpm(bpm) {}