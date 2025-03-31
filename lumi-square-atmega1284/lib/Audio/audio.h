#ifndef AUDIO_H
#define AUDIO_H

#include "avr/io.h"
#include "fixedupdate.h"
#include "musicnote.h"

enum DFPlayerCommand
{
    Zero = 0,
    One = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Eleven = 11,
    Twelve = 12,
    Thirteen = 13,
    Fourteen = 14,
    Fifteen = 15,
    Sixteen = 16,
    Seventeen = 17,
    Eighteen = 18,
    Nineteen = 19,
    Twenty = 20,
    Thirty = 21,
    Fourty = 22,
    Fifty = 23,
    Sixty = 24,
    Seventy = 25,
    Eighty = 26,
    Ninety = 27,
    OneHundred = 28,
    TwoHundred = 29,
    ThreeHundred = 30,
    FourHundred = 31,
    FiveHundred = 32,
    Score = 33,
    NewHighScore = 34,
    MemoryMatching = 35,
    Simon = 36,
    LightDash = 37,
    Easy = 38,
    Medium = 39,
    Hard = 40,
    Mute = 41,
    Unmute = 42,
    None = 255
};

class AudioSource : public FixedUpdateEventListener
{
private:
    const MusicNote *m_pQueuedAudioData;
    bool m_isPlayingAudioClip;
    bool m_isPlayingMusicNote;
    bool m_isMute;
    bool m_muteButtonPressed;
    int16_t m_audioPlayTime;
    uint16_t m_audioClipBeatDuration;
    int16_t m_muteButtonDelay;

public:
    static AudioSource &Instance();
    bool isMusicNoteSequencePlaying();
    bool isMusicNotePlaying();
    bool isVoiceOverPlaying();
    bool isMuteButtonsPressed();
    void updateMusicNoteTimer();
    void playAudioClip(MusicNoteSequence audioClip);
    void playNextAudioClipNote();
    void playMusicNote(MusicNote note, int16_t time);
    void playVoiceOver(DFPlayerCommand voiceOver);
    void queueNumberVoiceOver(uint16_t number);
    void queueVoiceOver(DFPlayerCommand voiceOver);
    void muteAudioSource(bool mute);
    void onFixedUpdate() override;

private:
    AudioSource();
    AudioSource(const AudioSource &) = delete;
    void operator=(const AudioSource &) = delete;
    void sendDFPlayerCommand(const uint8_t data[]);
};

#endif