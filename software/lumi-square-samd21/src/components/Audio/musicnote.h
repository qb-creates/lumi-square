#ifndef MUSICNOTESEQUENCE_H
#define MUSICNOTESEQUENCE_H

#include <stdint.h>

enum class MusicNote : int16_t
{
    C0 = 11478,
    Db0 = 10835,
    D0 = 10226,
    Eb0 = 9647,
    E0 = 9108,
    F0 = 8595,
    Gb0 = 8115,
    G0 = 7657,
    Ab0 = 7226,
    A0 = 6821,
    Bb0 = 6437,
    B0 = 6076,
    C1 = 5736,
    Db1 = 5413,
    D1 = 5109,
    Eb1 = 4823,
    E1 = 4552,
    F1 = 4297,
    Gb1 = 4055,
    G1 = 3827,
    Ab1 = 3612,
    A1 = 3409,
    Bb1 = 3218,
    B1 = 3037,
    C2 = 2867,
    Db2 = 2705,
    D2 = 2554,
    Eb2 = 2410,
    E2 = 2275,
    F2 = 2147,
    Gb2 = 2027,
    G2 = 1913,
    Ab2 = 1806,
    A2 = 1704,
    Bb2 = 1608,
    B2 = 1518,
    C3 = 1433,
    Db3 = 1353,
    D3 = 1276,
    Eb3 = 1205,
    E3 = 1137,
    F3 = 1073,
    Gb3 = 1013,
    G3 = 956,
    Ab3 = 903,
    A3 = 852,
    Bb3 = 804,
    B3 = 759,
    C4 = 716,
    Db4 = 676,
    D4 = 638,
    Eb4 = 602,
    E4 = 568,
    F4 = 536,
    Gb4 = 506,
    G4 = 478,
    Ab4 = 451,
    A4 = 425,
    Bb4 = 401,
    B4 = 379,
    C5 = 358,
    Db5 = 338,
    D5 = 319,
    Eb5 = 300,
    E5 = 284,
    F5 = 268,
    Gb5 = 253,
    G5 = 239,
    Ab5 = 225,
    A5 = 213,
    Bb5 = 200,
    B5 = 189,
    C6 = 179,
    Db6 = 169,
    D6 = 159,
    Eb6 = 150,
    E6 = 142,
    F6 = 134,
    Gb6 = 126,
    G6 = 119,
    Ab6 = 112,
    A6 = 106,
    Bb6 = 100,
    B6 = 94,
    C7 = 89,
    Db7 = 84,
    D7 = 79,
    Eb7 = 75,
    E7 = 71,
    F7 = 67,
    Gb7 = 63,
    G7 = 59,
    Ab7 = 56,
    A7 = 52,
    Bb7 = 49,
    B7 = 47,
    C8 = 44,
    Db8 = 42,
    D8 = 39,
    Eb8 = 37,
    E8 = 35,
    F8 = 33,
    Gb8 = 31,
    G8 = 29,
    Ab8 = 28,
    A8 = 26,
    Bb8 = 24,
    B8 = 23,
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