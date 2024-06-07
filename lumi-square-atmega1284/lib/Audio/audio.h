#ifndef AUDIO_H
#define AUDIO_H

#include "audioclip.h"
#include "avr/io.h"
#include "fixedupdate.h"

class AudioSource : public FixedUpdateEventListener
{
public:
    static AudioSource &Instance();
    bool isPlayingAudioClip();
    bool isPlayingMusicNote();
    bool isMuteButtonsPressed();
    void updateMusicNoteTimer();
    void playAudioClip(AudioClip audioClip);
    void playNextAudioClipNote();
    void playMusicNote(MusicNote note, int16_t time);
    void muteAudioSource(bool mute);
    void onFixedUpdate() override;

private:
    AudioSource();
    AudioSource(const AudioSource &) = delete;
    void operator=(const AudioSource &) = delete;
    const MusicNote *m_pQueuedAudioData;
    bool m_isPlayingAudioClip;
    bool m_isPlayingMusicNote;
    bool m_isMute;
    bool m_muteButtonPressed;
    int16_t m_audioPlayTime;
    uint16_t m_audioClipBeatDuration;
    int16_t m_muteButtonDelay;
};

#endif