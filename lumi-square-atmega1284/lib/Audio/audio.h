#ifndef AUDIO_H
#define AUDIO_H

#include "avr/io.h"

enum class MusicNote : uint16_t
{
    C3 = 880,
    G3 = 587,
    C4 = 439,
    E4 = 349,
    G4 = 292,
    C5 = 219,
    Db5 = 207,
    D5 = 196,
    Eb5 = 185,
    E5 = 174,
    F5 = 165,
    Gb5 = 155,
    G5 = 147,
    Ab5 = 138,
    A5 = 130,
    Bb5 = 123,
    B5 = 117,
    C6 = 110,
    Db6 = 104,
    D6 = 98,
    Eb = 92,
    G6 = 70
};

class AudioSource
{
public:
    static void configureAudioSource();
    static void updateAudioSource();
    static void playNote(MusicNote note, int16_t time);

private:
    AudioSource();
    static bool isPlaying;
    static int16_t playTime;
    static MusicNote note;
};

#endif

// for (int i = 0; i < 16; i++)
// {

//     Output::setLedColor(i, Colors::aquamarine);
//     Output::setLedIntensity(i, .1);
//     Output::ledOff(i);
// }
// while (1)
// {
//     for (int i = 0; i < 16; i++)
//     {
//         if (Input::getButton(i))
//         {
//             Output::ledOn(i);
//         }
//         else
//         {
//             Output::ledOff(i);
//         }
//     }
//     if (Input::getButton(0))
//     {
//         // OCR1A = 100;
//         OCR1A = 110;
//     }
//     else if (Input::getButton(1))
//     {
//         // OCR1A = 120;
//         OCR1A = 116;
//     }
//     else if (Input::getButton(2))
//     {
//         // OCR1A = 140;
//         OCR1A = 130;
//     }
//     else if (Input::getButton(3))
//     {
//         // OCR1A = 160;
//         OCR1A = 164;
//     }
//     else if (Input::getButton(4))
//     {
//         // OCR1A = 180;
//         OCR1A = 177;
//     }
//     else if (Input::getButton(5))
//     {
//         OCR1A = 200;
//     }
//     else if (Input::getButton(6))
//     {
//         OCR1A = 240;
//     }
//     else if (Input::getButton(7))
//     {
//         OCR1A = 250;
//     }
//     else
//     {
//         OCR1A = 0;
//     }
// }