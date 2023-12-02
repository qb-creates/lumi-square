#include "audio.h"

int16_t AudioSource::playTime = 0;
bool AudioSource::isPlaying = false;

void AudioSource::configureAudioSource()
{
    //  Configure
    DDRE |= _BV(PE3);

    // Enable PWM mode based on comparator OCR1B. Clear OC1B on match and set OC1B when TCNT1 = 0x000;
    TCCR3A |= _BV(WGM30) | _BV(WGM31);

    // Selects a Prescaler of 64. Fast PWM mode. Top is determined by OCR1A
    TCCR3B |= _BV(WGM33) | _BV(WGM32) | _BV(CS31) | _BV(CS30);
}

void AudioSource::updateAudioSource()
{
    if (playTime > 0)
    {
        playTime -= 16;

        if (playTime <= 0)
        {
            // Disable PWM output
            TCCR3A &= ~_BV(COM3A0);
            isPlaying = false;
        }
    }
}

void AudioSource::playNote(MusicNote note, int16_t time)
{
    OCR3A = static_cast<uint16_t>(note);

    // Enable PWM output
    TCCR3A |= _BV(COM3A0);
    playTime = time;
    isPlaying = true;
}