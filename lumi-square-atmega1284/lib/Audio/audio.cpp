#include "audio.h"

int16_t AudioSource::playTime = 0;
bool AudioSource::isPlaying = false;

void AudioSource::configureAudioSource()
{
    //  Configure
    DDRD |= _BV(PD4);

    // Enable PWM mode based on comparator OCR3A. Clear OC1B on compare match and set OC1B when TCNT3 = 0x000;
    // OCR1A is our top and how we adjust pwm frequency.
    // Frequency is going to be 
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
            TCCR1A &= ~_BV(COM1B1);
            isPlaying = false;
        }
    }
}

void AudioSource::playNote(MusicNote note, int16_t time)
{
    OCR1A = static_cast<uint16_t>(note) * 2;
    OCR1B = static_cast<uint16_t>(note);

    // Enable PWM output
    TCCR1A |= _BV(COM1B1);
    playTime = time;
    isPlaying = true;
}