#include "audio.h"
#include "buttons.h"
#include "deviceutility.h"

volatile DFPlayerCommand voiceOverQueue[10] = {DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None, DFPlayerCommand::None};
volatile int8_t voiceOverQueueCount = 0;
volatile bool isPlayingVoiceOver = false;

AudioSource::AudioSource()
    : m_pQueuedNoteSequenceData(nullptr),
      m_isPlayingNoteSequence(false),
      m_isPlayingMusicNote(false),
      m_isMute(false),
      m_muteButtonPressed(false),
      m_notePlayTime(0),
      m_noteSequenceBeatDuration(0),
      m_muteButtonDelay(0)
{
    DeviceUtility::Instance().processAudioCommand(DFPlayerCommand::Unmute);
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
    if (voiceOverQueueCount > 0 && !isPlayingVoiceOver)
    {
        isPlayingVoiceOver = true;
        DFPlayerCommand voiceOver = voiceOverQueue[0];
        DeviceUtility::Instance().processAudioCommand(voiceOver, [](){ isPlayingVoiceOver = false; });

        for (int8_t i = 0; i < voiceOverQueueCount - 1; ++i)
        {
            voiceOverQueue[i] = voiceOverQueue[i + 1];
        }

        --voiceOverQueueCount;
    }

    if (Input::getDifficultyButton() && m_muteButtonDelay < 1000)
    {
        m_muteButtonDelay += DeviceUtility::DELTA_TIME;

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
    // return voiceOverQueueCount > 0 || isPlayingVoiceOver;
    return false;
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
        DeviceUtility::Instance().setBeepNote(*m_pQueuedNoteSequenceData++);
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


        if (!m_isMute)
        {
            DeviceUtility::Instance().enableBeep(true);
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
    DeviceUtility::Instance().setBeepNote(note);

    if (!m_isMute)
    {
        DeviceUtility::Instance().enableBeep(true);
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
    DeviceUtility::Instance().processAudioCommand(voiceOver, []()
                                                  { isPlayingVoiceOver = false; });
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

    if ((ones != 0 && (ones + tens < 10 || ones + tens > 20)) || number == 0)
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
        DeviceUtility::Instance().enableBeep(false);
        playVoiceOver(DFPlayerCommand::MuteVoiceOver);
        return;
    }

    if (m_isPlayingMusicNote)
    {
        DeviceUtility::Instance().enableBeep(true);
    }

    DeviceUtility::Instance().processAudioCommand(DFPlayerCommand::Unmute);
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

    m_notePlayTime -= DeviceUtility::DELTA_TIME;

    if (m_notePlayTime <= 0)
    {
        m_isPlayingMusicNote = false;

        if (*m_pQueuedNoteSequenceData == MusicNote::END && m_isPlayingNoteSequence)
        {
            m_isPlayingNoteSequence = false;
        }

        DeviceUtility::Instance().enableBeep(false);
    }
}