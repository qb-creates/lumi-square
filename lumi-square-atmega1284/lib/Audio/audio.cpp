#include "audio.h"

int8_t AudioSource::playTime = 0;
bool AudioSource::isPlaying = false;

void AudioSource::configureAudioSource()
{
    //  Configure
    DDRB |= _BV(PB5);

    // Enable PWM mode based on comparator OCR1B. Clear OC1B on match and set OC1B when TCNT1 = 0x000;
    TCCR1A |= _BV(WGM10) | _BV(WGM11);

    // Selects a Prescaler of 64. Fast PWM mode. Top is determined by OCR1A
    TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
}

void AudioSource::updateAudioSource()
{
    if (playTime > 0)
    {
        playTime -= 16;

        if (playTime <= 0)
        {
            // Disable PWM output
            TCCR1A &= ~_BV(COM1A0);
            isPlaying = false;
        }
    }
}

void AudioSource::playNote(MusicNote note, int8_t time)
{
    OCR1A = static_cast<uint16_t>(note);

    // Enable PWM output
    TCCR1A |= _BV(COM1A0);
    playTime = time;
    isPlaying = true;
}