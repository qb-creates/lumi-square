#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "samd21j18a.h"
#include <string.h>

const uint8_t DF_PLAYER_COMMANDS[46][10] = {
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x01, 0xFE, 0xEA, 0xEF}, // 0
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
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x09, 0xFE, 0xDF, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x0A, 0xFE, 0xDE, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x0B, 0xFE, 0xDD, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xFE, 0xF5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x0F, 0xFE, 0xE6, 0xEF}};
const uint8_t MUTE_COMMAND_RESPONSE[20] = {0x7E, 0xFF, 0x06, 0x3D, 0x00, 0x00, 0x2B, 0xFE, 0x93, 0xEF, 0x7E, 0xFF, 0x06, 0x3D, 0x00, 0x00, 0x2B, 0xFE, 0x93, 0xEF};

volatile DFPlayerCommand voiceOverQueue[10] = {DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None};
volatile uint8_t receiveBufferIndex = 0;
volatile int8_t voiceOverQueueCount = 0;
volatile bool isPlayingVoiceOver = false;
volatile bool muteInProgress = false;
volatile uint8_t voiceOverBuffer[20];

extern "C" void SERCOM5_Handler(void)
{
    if (SERCOM5_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        uint8_t data = SERCOM5_REGS->USART_INT.SERCOM_DATA;
        voiceOverBuffer[receiveBufferIndex] = data;
        receiveBufferIndex++;
    
        if (receiveBufferIndex == 20 && data == 0xEF)
        {
            if (memcmp((const void *)voiceOverBuffer, MUTE_COMMAND_RESPONSE, sizeof(voiceOverBuffer)) == 0)
            {
                muteInProgress = true;
            }
    
            receiveBufferIndex = 0;
            isPlayingVoiceOver = false;
        }

        SERCOM5_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_RXC_Msk;
    }
}

AudioSource::AudioSource()
    : FixedUpdateEventListener(),
      m_pQueuedNoteSequenceData(nullptr),
      m_isPlayingNoteSequence(false),
      m_isPlayingMusicNote(false),
      m_isMute(false),
      m_muteButtonPressed(false),
      m_notePlayTime(0),
      m_noteSequenceBeatDuration(0),
      m_muteButtonDelay(0)
{

    // Configure PWM for music tones
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC7_Msk;
    
    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC6_TC7 | GCLK_CLKCTRL_CLKEN_Msk;

    PORT_REGS->GROUP[1].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[1].PORT_PMUX[11] = PORT_PMUX_PMUXO_E;
    
    TC7_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MPWM | TC_CTRLA_PRESCALER_DIV256;
    while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}

    // // Configure SERCOM5 USart for DFPlayer mini
    // Turn on clock for usart
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM5_Msk;

    // Configure clocks for timer and interrupts
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM5_CORE | GCLK_CLKCTRL_CLKEN_Msk;

    // Enable pin multiplexer for PA20 and configure it to be function C Pad 0
    PORT_REGS->GROUP[0].PORT_PINCFG[20] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[0].PORT_PMUX[10] |= PORT_PMUX_PMUXE_C;

    // Enable pin multiplexer for PA12 and configure it to be function C Pad 0
    PORT_REGS->GROUP[0].PORT_PINCFG[21] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[0].PORT_PMUX[10] |= PORT_PMUX_PMUXO_C;

    SERCOM5_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_TXEN_Msk | SERCOM_USART_INT_CTRLB_RXEN_Msk;
    while ((SERCOM5_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U) { }
    
    // Sets the baud rate to 9600
    SERCOM5_REGS->USART_INT.SERCOM_BAUD = 0xFF2E;
    SERCOM5_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk;
    SERCOM5_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_ENABLE_Msk | SERCOM_USART_INT_CTRLA_DORD_LSB | SERCOM_USART_INT_CTRLA_TXPO(1) | SERCOM_USART_INT_CTRLA_RXPO(3) | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK;
    NVIC_EnableIRQ(SERCOM5_IRQn);

    sendDFPlayerCommand(DF_PLAYER_COMMANDS[DFPlayerCommand::Unmute]);
}

AudioSource &AudioSource::Instance()
{
    static AudioSource instance;
    return instance;
}

/**
 * @brief
 *
 */
void AudioSource::onFixedUpdate()
{
    if (muteInProgress)
    {
        sendDFPlayerCommand(DF_PLAYER_COMMANDS[DFPlayerCommand::Mute]);
        muteInProgress = false;
    }

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

    playNextMusicSequenceNote();
    updateMusicNoteTimer();
}

/**
 * @brief Checks if a music note sequence is currently playing.
 *
 * Retrieves the current state of music note sequence playback. Returns true if an music note sequence
 * is currently playing, and false otherwise.
 *
 * @return true if an music note sequence is currently playing, false otherwise.
 */
bool AudioSource::isMusicNoteSequencePlaying()
{
    return m_isPlayingNoteSequence;
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
bool AudioSource::isMuteButtonPressed()
{
    return m_muteButtonPressed;
}

/**
 * @brief Prepares and initiates playback of the given music note sequence.
 *
 * Sets the beat duration based on the clip's beats per minute (bpm).
 * Initializes queued music note sequence data and marks clip for immediate playback.
 * Playback of individual notes from the queued music note sequence is initiated
 * by calling playNextMusicSequenceNote() every 16ms.
 *
 * @param musicNoteSequence The music note sequence to be queued for playback.
 */
void AudioSource::playNoteSequence(MusicNoteSequence musicNoteSequence)
{
    m_noteSequenceBeatDuration = 60000 / musicNoteSequence.bpm;
    m_isPlayingNoteSequence = true;
    m_pQueuedNoteSequenceData = musicNoteSequence.audioData;
}

/**
 * @brief Plays the next note from the music note sequence data.
 *
 * This function plays the next note from the music note sequence data. It retrieves the
 * next note and its duration from the queued audio data. Then, it calculates
 * the play time duration for the note based on the music note sequence's calculated beat duration.
 * The function loads the note's OCR value into the OCR1 register to set the PWM
 * frequency for that note. The note is played for the calculated note duration.
 */
void AudioSource::playNextMusicSequenceNote()
{
    if (!m_isPlayingMusicNote && m_isPlayingNoteSequence)
    {
        int16_t note = static_cast<int16_t>(*m_pQueuedNoteSequenceData++);
        int16_t noteDuration = static_cast<int16_t>(*m_pQueuedNoteSequenceData++);

        if (noteDuration < 0)
        {
            noteDuration *= -1;
            m_notePlayTime = ((m_noteSequenceBeatDuration * 4) / noteDuration) + ((m_noteSequenceBeatDuration * 4) / (noteDuration * 2));
        }
        else
        {
            m_notePlayTime = (m_noteSequenceBeatDuration * 4) / noteDuration;
        }

        // Configure pwm frequency
        TC7_REGS->COUNT16.TC_CC[0] = static_cast<uint16_t>(note);
        TC7_REGS->COUNT16.TC_CC[1] = static_cast<uint16_t>(note) / 2;
        TC7_REGS->COUNT16.TC_COUNT = 0;

        if (!m_isMute)
        {
            // Enable PWM output
            TC7_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
            while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
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
    TC7_REGS->COUNT16.TC_CC[0] = static_cast<uint16_t>(note);
    TC7_REGS->COUNT16.TC_CC[1] = static_cast<uint16_t>(note) / 2;
    TC7_REGS->COUNT16.TC_COUNT = 0;

    if (!m_isMute)
    {
        // Enable PWM output
        TC7_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
        while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    }

    m_notePlayTime = time;
    m_isPlayingMusicNote = true;
}

/**
 * @brief
 *
 * @param voiceOver
 */
void AudioSource::playVoiceOver(DFPlayerCommand voiceOver)
{
    if (m_isMute && voiceOver != DFPlayerCommand::MuteVoiceOver && voiceOver != DFPlayerCommand::MuteVoiceOver)
        return;

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
    if (m_isMute)
        return;

    uint16_t hundreds = (number / 100) * 100;
    uint8_t tens = ((number % 100) / 10) * 10;
    uint8_t ones = number % 10;

    if (hundreds != 0)
    {
        uint8_t mappedHundred = (hundreds / 100) + 27;
        queueVoiceOver(static_cast<DFPlayerCommand>(mappedHundred));
    }

    if (tens != 0)
    {
        uint8_t mappedTens = tens == 10 ? (tens + ones) : (tens / 10) + 18;
        queueVoiceOver(static_cast<DFPlayerCommand>(mappedTens));
    }

    if ((ones != 0  && (ones + tens < 10 || ones + tens > 20)) || number == 0)
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

    if (m_isMute && voiceOver != DFPlayerCommand::MuteVoiceOver && voiceOver != DFPlayerCommand::UnmuteVoiceOver)
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
        TC7_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
        while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
        playVoiceOver(DFPlayerCommand::MuteVoiceOver);
        return;
    }

    if (m_isPlayingMusicNote)
    {
        TC7_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
        while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    }        

    sendDFPlayerCommand(DF_PLAYER_COMMANDS[DFPlayerCommand::Unmute]);
    queueVoiceOver(DFPlayerCommand::UnmuteVoiceOver);
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

    m_notePlayTime -= FixedUpdateTimer::DELTA_TIME;

    if (m_notePlayTime <= 0)
    {
        m_isPlayingMusicNote = false;

        if (*m_pQueuedNoteSequenceData == MusicNote::END && m_isPlayingNoteSequence)
        {
            m_isPlayingNoteSequence = false;
        }

        // Disable PWM output
        TC7_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
        while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    }
}

void AudioSource::sendDFPlayerCommand(const uint8_t data[])
{
    for (size_t i = 0; i < 10; i++)
    {
        while (!(SERCOM5_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk)) {}        
        SERCOM5_REGS->USART_INT.SERCOM_DATA = data[i];
    }
}
