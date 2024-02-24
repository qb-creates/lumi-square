#ifndef AUDIO_H
#define AUDIO_H

#include "audioclip.h"
#include "avr/io.h"

class AudioSource
{
public:
    static bool isPlayingAudioClip();
    static bool isPlayingMusicNote();
    static void configureAudioSource();
    static void updateMusicNoteTimer();
    static void playAudioClip(AudioClip audioClip);
    static void playNextAudioClipNote();
    static void playMusicNote(MusicNote note, int16_t time);

private:
    AudioSource();
    AudioSource(const AudioSource &) = delete;
    void operator=(const AudioSource &) = delete;
    static const MusicNote *m_pQueuedAudioData;
    static bool m_isPlayingAudioClip;
    static bool m_isPlayingMusicNote;
    static int16_t m_audioPlayTime;
    static uint16_t m_audioClipBeatDuration;
};

#endif