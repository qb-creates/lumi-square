#ifndef AUDIO_H
#define AUDIO_H

#include "musicnote.h"
#include "audiocommand.h"

class AudioSource
{
private:
    const MusicNote *m_pQueuedNoteSequenceData;
    bool m_isPlayingNoteSequence;
    bool m_isPlayingMusicNote;
    bool m_isMute;
    bool m_muteButtonPressed;
    int16_t m_notePlayTime;
    uint16_t m_noteSequenceBeatDuration;
    int16_t m_muteButtonDelay;

public:
    static AudioSource &Instance();
    void onFixedUpdate();
    bool isMusicNoteSequencePlaying();
    bool isMusicNotePlaying();
    bool isVoiceOverPlaying();
    bool isMuteButtonPressed();
    void playNoteSequence(MusicNoteSequence musicNoteSequence);
    void playNextMusicSequenceNote();
    void playMusicNote(MusicNote note, int16_t time);
    void playVoiceOver(DFPlayerCommand voiceOver);
    void queueNumberVoiceOver(uint16_t number);
    void queueVoiceOver(DFPlayerCommand voiceOver);
    void muteAudioSource(bool mute);
    
    private:
    AudioSource();
    AudioSource(const AudioSource &) = delete;
    void operator=(const AudioSource &) = delete;
    void updateMusicNoteTimer();
};

#endif