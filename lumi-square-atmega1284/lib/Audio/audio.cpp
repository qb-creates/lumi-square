#include "audio.h"
#include "buttons.h"
#include "../STD/fixedupdate.h"
#include "../Peripherals/lcd.h"

AudioSource::AudioSource()
    : FixedUpdateEventListener(),
      m_pQueuedAudioData(nullptr),
      m_isPlayingAudioClip(false),
      m_isPlayingMusicNote(false),
      m_isMute(false),
      m_audioPlayTime(0),
      m_audioClipBeatDuration(0)
{
    DDRD |= _BV(PD4);
    TCCR1A |= _BV(WGM10) | _BV(WGM11);
    TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
}

AudioSource &AudioSource::Instance()
{
    static AudioSource instance;
    return instance;
}

/**
 * @brief Checks if an audio clip is currently playing.
 *
 * Retrieves the current state of audio clip playback. Returns true if an audio
 * clip is currently playing, and false otherwise.
 *
 * @return true if an audio clip is currently playing, false otherwise.
 */
bool AudioSource::isPlayingAudioClip()
{
    return m_isPlayingAudioClip;
}

/**
 * @brief Checks if a music note is currently playing.
 *
 * Retrieves the current state of music note playback. Returns true if a music
 * note is currently playing, and false otherwise.
 *
 * @return true if a music note is currently playing, false otherwise.
 */
bool AudioSource::isPlayingMusicNote()
{
    return m_isPlayingMusicNote;
}

/**
 * @brief Updates the timer for note playback.
 *
 * This function is responsible for updating the timer associated with note playback.
 * It decrements the timer value and stops note playback when the timer reaches zero.
 */
void AudioSource::updateMusicNoteTimer()
{
    if (!m_isPlayingMusicNote)
        return;

    m_audioPlayTime -= 16;

    if (m_audioPlayTime <= 0)
    {
        m_isPlayingMusicNote = false;

        if (*m_pQueuedAudioData == MusicNote::END && m_isPlayingAudioClip)
        {
            m_isPlayingAudioClip = false;
        }

        // Disable PWM output
        TCCR1A &= ~_BV(COM1B1);
    }
}

/**
 * @brief Prepares and initiates playback of the given audio clip.
 *
 * Sets the beat duration based on the clip's beats per minute (bpm).
 * Initializes queued audio data and marks clip for immediate playback.
 * Playback of individual notes from the queued audio clip is initiated
 * by calling playNextAudioClipNote() every 16ms.
 *
 * @param audioClip The audio clip to be queued for playback.
 */
void AudioSource::playAudioClip(AudioClip audioClip)
{
    m_audioClipBeatDuration = 60000 / audioClip.bpm;
    m_isPlayingAudioClip = true;
    m_pQueuedAudioData = audioClip.audioData;
}

/**
 * @brief Plays the next note from the audio clip data.
 *
 * This function plays the next note from the audio clip data. It retrieves the
 * next note and its duration from the queued audio data. Then, it calculates
 * the play time duration for the note based on the audio clip's calculated beat duration.
 * The function loads the note's OCR value into the OCR1 register to set the PWM
 * frequency for that note. The note is played for the calculated note duration.
 */
void AudioSource::playNextAudioClipNote()
{
    if (!m_isPlayingMusicNote && m_isPlayingAudioClip)
    {
        int16_t note = static_cast<int16_t>(*m_pQueuedAudioData++);
        int16_t noteDuration = static_cast<int16_t>(*m_pQueuedAudioData++);

        if (noteDuration < 0)
        {
            noteDuration *= -1;
            m_audioPlayTime = ((m_audioClipBeatDuration * 4) / noteDuration) + ((m_audioClipBeatDuration * 4) / (noteDuration * 2));
        }
        else
        {
            m_audioPlayTime = (m_audioClipBeatDuration * 4) / noteDuration;
        }

        // Configure pwm frequency
        OCR1A = note;
        OCR1B = note / 2;

        if (!m_isMute)
        {
            // Enable PWM output
            TCCR1A |= _BV(COM1B1);
        }

        m_isPlayingMusicNote = true;
    }
}

/**
 * @brief Plays the specified music note for the given duration.
 *
 * Starts immediate playback of the specified music note for the given duration.
 * Stops playback when the specified time elapses.
 *
 * @param note The music note to be played.
 * @param time The duration of note playback (in milliseconds).
 */
void AudioSource::playMusicNote(MusicNote note, int16_t time)
{
    OCR1A = static_cast<uint16_t>(note);
    OCR1B = static_cast<uint16_t>(note) / 2;

    if (!m_isMute)
    {
        // Enable PWM output
        TCCR1A |= _BV(COM1B1);
    }

    m_audioPlayTime = time;
    m_isPlayingMusicNote = true;
}

/**
 * @brief
 *
 * @param mute
 */
void AudioSource::muteAudioSource(bool mute)
{
    m_isMute = mute;

    mute ? LCD::Instance().writeByte(0, 15, 0x02) : LCD::Instance().writeByte(0, 15, 0x01);

    if (mute)
    {
        TCCR1A &= ~_BV(COM1B1);
    }
    else if (m_isPlayingMusicNote)
    {
        TCCR1A |= _BV(COM1B1);
    }
}

void AudioSource::onFixedUpdate()
{
    if (Input::getMuteButtonDown())
    {
        m_isMute = !m_isMute;
        muteAudioSource(m_isMute);
    }

    playNextAudioClipNote();
    updateMusicNoteTimer();
}
