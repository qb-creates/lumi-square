#include "audio.h"

int16_t AudioSource::playTime = 0;
bool AudioSource::isPlaying = false;
bool AudioSource::buttonIsPlaying = false;

int16_t happyBirthday[] = {
  static_cast<int16_t>(MusicNote::E4), 4,  static_cast<int16_t>(MusicNote::B3),8,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::D4),4,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::B3),8,
  static_cast<int16_t>(MusicNote::A3), 4,  static_cast<int16_t>(MusicNote::A3),8,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::E4),4,  static_cast<int16_t>(MusicNote::D4),8,  static_cast<int16_t>(MusicNote::C4),8,
  static_cast<int16_t>(MusicNote::B3), -4,  static_cast<int16_t>(MusicNote::C4),8,  static_cast<int16_t>(MusicNote::D4),4,  static_cast<int16_t>(MusicNote::E4),4,
  static_cast<int16_t>(MusicNote::C4), 4,  static_cast<int16_t>(MusicNote::A3),4,  static_cast<int16_t>(MusicNote::A3),8,  static_cast<int16_t>(MusicNote::A3),4,  static_cast<int16_t>(MusicNote::B3),8,  static_cast<int16_t>(MusicNote::C4),8,

  static_cast<int16_t>(MusicNote::D5), -4,  static_cast<int16_t>(MusicNote::F5),8,  static_cast<int16_t>(MusicNote::A5),4,  static_cast<int16_t>(MusicNote::G5),8,  static_cast<int16_t>(MusicNote::F5),8,
  static_cast<int16_t>(MusicNote::E5), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),4, static_cast<int16_t>(MusicNote::Rest), 4,

  static_cast<int16_t>(MusicNote::E5), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::B4),8,
  static_cast<int16_t>(MusicNote::A4), 4,  static_cast<int16_t>(MusicNote::A4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),8,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,

  static_cast<int16_t>(MusicNote::D5), -4,  static_cast<int16_t>(MusicNote::F5),8,  static_cast<int16_t>(MusicNote::A5),4,  static_cast<int16_t>(MusicNote::G5),8,  static_cast<int16_t>(MusicNote::F5),8,
  static_cast<int16_t>(MusicNote::E5), -4,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::D5),8,  static_cast<int16_t>(MusicNote::C5),8,
  static_cast<int16_t>(MusicNote::B4), 4,  static_cast<int16_t>(MusicNote::B4),8,  static_cast<int16_t>(MusicNote::C5),8,  static_cast<int16_t>(MusicNote::D5),4,  static_cast<int16_t>(MusicNote::E5),4,
  static_cast<int16_t>(MusicNote::C5), 4,  static_cast<int16_t>(MusicNote::A4),4,  static_cast<int16_t>(MusicNote::A4),4, static_cast<int16_t>(MusicNote::Rest), 4,
  

  static_cast<int16_t>(MusicNote::E5),2,  static_cast<int16_t>(MusicNote::C5),2,
  static_cast<int16_t>(MusicNote::D5),2,   static_cast<int16_t>(MusicNote::B4),2,
  static_cast<int16_t>(MusicNote::C5),2,   static_cast<int16_t>(MusicNote::A4),2,
  static_cast<int16_t>(MusicNote::Ab4),2,  static_cast<int16_t>(MusicNote::B4),4,  static_cast<int16_t>(MusicNote::Rest),8, 
  static_cast<int16_t>(MusicNote::E5),2,   static_cast<int16_t>(MusicNote::C5),2,
  static_cast<int16_t>(MusicNote::D5),2,   static_cast<int16_t>(MusicNote::B4),2,
  static_cast<int16_t>(MusicNote::C5),4,   static_cast<int16_t>(MusicNote::E5),4,  static_cast<int16_t>(MusicNote::A5),2,
  static_cast<int16_t>(MusicNote::Ab4),2,
};

void AudioSource::configureAudioSource()
{
    //  Configure
    DDRD |= _BV(PD4) | _BV(PD6);

    // Enable PWM mode based on comparator OCR3A. Clear OC1B on compare match and set OC1B when TCNT3 = 0x000;
    // OCR1A is our top and how we adjust pwm frequency.
    // Frequency is going to be
    TCCR1A |= _BV(WGM10) | _BV(WGM11);

    // Selects a Prescaler of 64. Fast PWM mode. Top is determined by OCR1A
    TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);

    TCCR2A |= _BV(WGM21) | _BV(WGM20);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20) | _BV(WGM22);
}
int musicCounter = 0;
void AudioSource::playMusic()
{
    if (!isPlaying)
    {
        int16_t noteDuration = happyBirthday[musicCounter + 1];
        int16_t duration = 0;
        if (noteDuration < 0)
        {
            noteDuration *= -1;
            duration = (1333 / noteDuration) + (1333 / (noteDuration * 2));
        }
        else
        {
            duration = (1333 / noteDuration);
        }

        playNote(happyBirthday[musicCounter], duration);

        musicCounter += 2;
        if (musicCounter > 197)
        {
            musicCounter = 0;
        }
    }
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
    OCR1A = static_cast<uint16_t>(note);
    OCR1B = static_cast<uint16_t>(note) / 2;

    // Enable PWM output
    TCCR1A |= _BV(COM1B1);
    playTime = time;
    isPlaying = true;
}

void AudioSource::playNote(int16_t note, int16_t time)
{
    OCR1A = note;
    OCR1B = note / 2;

    // Enable PWM output
    TCCR1A |= _BV(COM1B1);
    playTime = time;
    isPlaying = true;
}
