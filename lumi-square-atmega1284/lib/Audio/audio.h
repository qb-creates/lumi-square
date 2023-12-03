#ifndef AUDIO_H
#define AUDIO_H

#include "avr/io.h"

enum class MusicNote : uint16_t
{
    C3 = 2200,
    G3 = 1468,
    C4 = 1099,
    E4 = 872,
    G4 = 733,
    C5 = 549,
    Db5 = 518,
    D5 = 489,
    Eb5 = 461,
    E5 = 435,
    F5 = 411,
    Gb5 = 388,
    G5 = 366,
    Ab5 = 345,
    A5 = 326,
    Bb5 = 307,
    B5 = 290,
    C6 = 274,
    Db6 = 0,
    D6 = 244,
    Eb = 0,
    G6 = 182
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