#include "audio.h"
#include "../Utilities/fixedupdate.h"
#include "buttons.h"
#include <avr/interrupt.h>

const uint8_t DF_PLAYER_COMMANDS[43][10] = {
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x01, 0xFE, 0xEA, 0xEF},// 0
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x02, 0xFE, 0xE9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x03, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x04, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x05, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x06, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x07, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x08, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x09, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x0A, 0xFE, 0xE1, 0xEF}, 
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x01, 0xFE, 0xE9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x02, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x03, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x04, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x05, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x06, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x07, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x08, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x09, 0xFE, 0xE1, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0A, 0xFE, 0xE0, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0B, 0xFE, 0xDF, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0C, 0xFE, 0xDE, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0D, 0xFE, 0xDD, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0E, 0xFE, 0xDC, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0F, 0xFE, 0xDB, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x10, 0xFE, 0xDA, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x11, 0xFE, 0xD9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x12, 0xFE, 0xD8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x01, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x02, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x03, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x04, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x05, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x01, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x02, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x03, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x04, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x05, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x06, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x07, 0xFE, 0xE1, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x08, 0xFE, 0xE0, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xFE, 0xF5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x0F, 0xFE, 0xE6, 0xEF}
};
volatile DFPlayerCommand voiceOverQueue[10] = {DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None};
volatile uint8_t receiveBuffer[10];
volatile uint8_t receiveBufferIndex = 0;
volatile int8_t voiceOverQueueCount = 0;
volatile bool isPlayingVoiceOver = false;

ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;
    receiveBufferIndex++;

    if (receiveBufferIndex == 20 && data == 0xEF)
    {
        receiveBufferIndex = 0;
        isPlayingVoiceOver = false;
    }
}

AudioSource::AudioSource()
    : FixedUpdateEventListener(),
      m_pQueuedAudioData(nullptr),
      m_isPlayingAudioClip(false),
      m_isPlayingMusicNote(false),
      m_isMute(false),
      m_muteButtonPressed(false),
      m_audioPlayTime(0),
      m_audioClipBeatDuration(0),
      m_muteButtonDelay(0)
{

    // Configure PWM for music tones
    DDRD |= _BV(PD4);
    TCCR1A |= _BV(WGM10) | _BV(WGM11);
    TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);

    // Configure USART1 for DFPlayer mini
    UBRR1L = 119;
    UCSR1B = _BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1);
    UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);

    sendDFPlayerCommand(DF_PLAYER_COMMANDS[DFPlayerCommand::Unmute]);
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
bool AudioSource::isMusicNoteSequencePlaying()
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
bool AudioSource::isMusicNotePlaying()
{
    return m_isPlayingMusicNote;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool AudioSource::isVoiceOverPlaying()
{
    return voiceOverQueueCount > 0 || isPlayingVoiceOver;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool AudioSource::isMuteButtonsPressed()
{
    return m_muteButtonPressed;
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

    m_audioPlayTime -= FixedUpdateTimer::DELTA_TIME;

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
void AudioSource::playAudioClip(MusicNoteSequence audioClip)
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
 * @param voiceOver
 */
void AudioSource::playVoiceOver(DFPlayerCommand voiceOver)
{
    isPlayingVoiceOver = true;
    sendDFPlayerCommand(DF_PLAYER_COMMANDS[voiceOver]);
}

/**
 * @brief
 *
 * @param number
 */
void AudioSource::queueNumberVoiceOver(uint16_t number)
{
    uint16_t hundreds = (number / 100) * 100;
    uint8_t tens = ((number % 100) / 10) * 10;
    uint8_t ones = number % 10;

    if (hundreds != 0)
    {
        switch (hundreds)
        {
        case 100:
            queueVoiceOver(DFPlayerCommand::OneHundred);
            break;
        case 200:
            queueVoiceOver(DFPlayerCommand::TwoHundred);
            break;
        case 300:
            queueVoiceOver(DFPlayerCommand::ThreeHundred);
            break;
        case 400:
            queueVoiceOver(DFPlayerCommand::FourHundred);
            break;
        case 500:
            queueVoiceOver(DFPlayerCommand::FiveHundred);
            break;
        }
    }

    if (tens != 0)
    {
        switch (tens)
        {
        case 10:
            queueVoiceOver(static_cast<DFPlayerCommand>(tens + ones));
            return;
        case 20:
            queueVoiceOver(DFPlayerCommand::Twenty);
            break;
        case 30:
            queueVoiceOver(DFPlayerCommand::Thirty);
            break;
        case 40:
            queueVoiceOver(DFPlayerCommand::Fourty);
            break;
        case 50:
            queueVoiceOver(DFPlayerCommand::Fifty);
            break;
        case 60:
            queueVoiceOver(DFPlayerCommand::Sixty);
            break;
        case 70:
            queueVoiceOver(DFPlayerCommand::Seventy);
            break;
        case 80:
            queueVoiceOver(DFPlayerCommand::Eighty);
            break;
        case 90:
            queueVoiceOver(DFPlayerCommand::Ninety);
            break;
        }
    }

    if (ones != 0)
        queueVoiceOver(static_cast<DFPlayerCommand>(ones));
}

/**
 * @brief
 *
 * @param voiceOver
 */
void AudioSource::queueVoiceOver(DFPlayerCommand voiceOver)
{
    if (voiceOverQueueCount == 10)
        return;

    voiceOverQueue[voiceOverQueueCount] = voiceOver;
    ++voiceOverQueueCount;
}

/**
 * @brief
 *
 * @param mute
 */
void AudioSource::muteAudioSource(bool mute)
{
    m_isMute = mute;

    if (mute)
    {
        TCCR1A &= ~_BV(COM1B1);        
    }
    else if (m_isPlayingMusicNote)
    {
        TCCR1A |= _BV(COM1B1);
    }

    DFPlayerCommand command = mute ? DFPlayerCommand::Mute : DFPlayerCommand::Unmute;
    sendDFPlayerCommand(DF_PLAYER_COMMANDS[command]);
}

void AudioSource::onFixedUpdate()
{
    if (voiceOverQueueCount > 0 && !isPlayingVoiceOver)
    {
        isPlayingVoiceOver = true;
        DFPlayerCommand voiceOver = voiceOverQueue[0];
        sendDFPlayerCommand(DF_PLAYER_COMMANDS[voiceOver]);

        for (int8_t i = 0; i < voiceOverQueueCount - 1; ++i)
        {
            voiceOverQueue[i] = voiceOverQueue[i + 1];
        }

        --voiceOverQueueCount;
    }

    if (Input::getDifficultyButton() && m_muteButtonDelay < 1000)
    {
        m_muteButtonDelay += FixedUpdateTimer::DELTA_TIME;

        if (m_muteButtonDelay >= 1000)
        {
            m_isMute = !m_isMute;
            muteAudioSource(m_isMute);
            m_muteButtonPressed = true;
        }
    }

    if (!Input::getDifficultyButton())
    {
        m_muteButtonDelay = 0;
        m_muteButtonPressed = false;
    }

    playNextAudioClipNote();
    updateMusicNoteTimer();
}

void AudioSource::sendDFPlayerCommand(const uint8_t data[])
{
    for (uint8_t i = 0; i < 10; ++i)
    {
        // Wait until the Transmitter is ready
        loop_until_bit_is_set(UCSR1A, UDRE1);
        UDR1 = data[i];
    }
}
