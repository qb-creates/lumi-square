#ifndef MUSICNOTESEQUENCE_H
#define MUSICNOTESEQUENCE_H

#include <stdint.h>

enum class MusicNote : int16_t
{
    C0 = 17631,
    Db0 = 16643,
    D0 = 15708,
    Eb0 = 14819,
    E0 = 13991,
    F0 = 13202,
    Gb0 = 12465,
    G0 = 11762,
    Ab0 = 11100,
    A0 = 10478,
    Bb0 = 9888,
    B0 = 9334,
    C1 = 8811,
    Db1 = 8315,
    D1 = 7848,
    Eb1 = 7408,
    E1 = 6992,
    F1 = 6600,
    Gb1 = 6229,
    G1 = 5879,
    Ab1 = 5549,
    A1 = 5237,
    Bb1 = 4943,
    B1 = 4665,
    C2 = 4404,
    Db2 = 4156,
    D2 = 3923,
    Eb2 = 3703,
    E2 = 3495,
    F2 = 3299,
    Gb2 = 3114,
    G2 = 2939,
    Ab2 = 2774,
    A2 = 2618,
    Bb2 = 2471,
    B2 = 2332,
    C3 = 2201,
    Db3 = 2078,
    D3 = 1961,
    Eb3 = 1851,
    E3 = 1747,
    F3 = 1649,
    Gb3 = 1556,
    G3 = 1469,
    Ab3 = 1387,
    A3 = 1309,
    Bb3 = 1235,
    B3 = 1166,
    C4 = 1100,
    Db4 = 1039,
    D4 = 980,
    Eb4 = 925,
    E4 = 873,
    F4 = 824,
    Gb4 = 778,
    G4 = 734,
    Ab4 = 693,
    A4 = 654,
    Bb4 = 617,
    B4 = 583,
    C5 = 550,
    Db5 = 519,
    D5 = 490,
    Eb5 = 462,
    E5 = 436,
    F5 = 412,
    Gb5 = 389,
    G5 = 367,
    Ab5 = 346,
    A5 = 327,
    Bb5 = 308,
    B5 = 291,
    C6 = 275,
    Db6 = 259,
    D6 = 245,
    Eb6 = 231,
    E6 = 218,
    F6 = 206,
    Gb6 = 194,
    G6 = 183,
    Ab6 = 173,
    A6 = 163,
    Bb6 = 154,
    B6 = 145,
    C7 = 137,
    Db7 = 129,
    D7 = 122,
    Eb7 = 115,
    E7 = 109,
    F7 = 103,
    Gb7 = 97,
    G7 = 91,
    Ab7 = 86,
    A7 = 81,
    Bb7 = 77,
    B7 = 72,
    C8 = 68,
    Db8 = 64,
    D8 = 61,
    Eb8 = 57,
    E8 = 54,
    F8 = 51,
    Gb8 = 48,
    G8 = 45,
    Ab8 = 43,
    A8 = 40,
    Bb8 = 38,
    B8 = 36,
    Rest = 0,
    Whole = 1,
    Half = 2,
    Quarter = 4,
    Eighth = 8,
    DottedWhole = -1,
    DottedHalf = -2,
    DottedQuarter = -4,
    DottedEighth = -8,
    END = -100
};

class MusicNoteSequence
{
public:
    const MusicNote *audioData;
    uint8_t bpm;
    MusicNoteSequence(const MusicNote *audioData, uint8_t bpm);
    void operator=(const MusicNoteSequence &) = delete;
};

const MusicNote POWER_UP_SOUND_EFFECT_DATA[] = {
    MusicNote::C4, MusicNote::Eighth,
    MusicNote::E4, MusicNote::Eighth,
    MusicNote::G4, MusicNote::Eighth,
    MusicNote::C5, MusicNote::Eighth,
    MusicNote::E5, MusicNote::Eighth,
    MusicNote::END
};

const MusicNote FAILURE_SOUND_EFFECT_DATA[] = {
    MusicNote::D3, MusicNote::Eighth,
    MusicNote::B2, MusicNote::Quarter,
    MusicNote::END
};

const MusicNoteSequence FAILURE_AUDIO_CLIP(FAILURE_SOUND_EFFECT_DATA, 130);
const MusicNoteSequence POWER_UP_AUDIO_CLIP(POWER_UP_SOUND_EFFECT_DATA, 240);

#endif