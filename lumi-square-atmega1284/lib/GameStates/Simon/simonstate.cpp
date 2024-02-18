#include "simonstate.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(),
      sequenceIndex(0),
      activeLedIndex(0),
      currentRound(0),
      simonLedSequence{},
      simonMusicNoteSequence{},
      ledOptions{5, 6, 9, 10, 0, 3, 12, 15},
      musicNoteOptions{MusicNote::G3, MusicNote::C3, MusicNote::E3, MusicNote::G2, MusicNote::G4, MusicNote::C4, MusicNote::E4, MusicNote::G3},
      listeningForPlayerInput(false),
      ledBrightnessAdjustTimer(0) {}

void SimonState::enterState()
{
    lives = GameProperties::Instance().gameDifficulty == Difficulty::Hard ? 1 : 3;

    if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
    {
        Output::ledOn(0, Colors::aquamarine, .1);
        Output::ledOn(3, Colors::pink, .1);
        Output::ledOn(12, Colors::purple, .1);
        Output::ledOn(15, Colors::orange, .1);
    }

    Output::ledOn(5, Colors::green, .1);
    Output::ledOn(6, Colors::red, .1);
    Output::ledOn(9, Colors::yellow, .1);
    Output::ledOn(10, Colors::azure, .1);

    LCD::Instance().writeString(0, 0, " Round   Lives \x02");
    LCD::Instance().writeString(1, 0, "    1           ");

    for (int i = 0; i < lives; ++i)
    {
        LCD::Instance().writeByte(1, 12 - i, 0x01);
    }

    playRoundSequence();
}

void SimonState::exitState()
{
    uint8_t allSimonButtons[] = {5, 6, 9, 10, 0, 3, 12, 15};
    Output::ledOn(allSimonButtons, 8, Colors::red, .6);

    ledBrightnessAdjustTimer = 0;
    currentRound = 0;
    sequenceIndex = 0;
    nextState = GameState::None;
}

void SimonState::updateState()
{
    if (ledBrightnessAdjustTimer <= 0 || nextState == GameState::GameOver)
        return;

    ledBrightnessAdjustTimer -= 16;

    if (ledBrightnessAdjustTimer >= 0)
        return;

    if (Output::getLedIntensity(activeLedIndex) == .6)
    {
        ledBrightnessAdjustTimer = 200;
        Output::setLedIntensity(activeLedIndex, .1);
        return;
    }

    if (sequenceIndex == currentRound)
    {
        listeningForPlayerInput ? playRoundSequence() : listenForPlayerInput();
        return;
    }

    if (!listeningForPlayerInput)
    {
        playNextSequenceNote();
    }
}

void SimonState::onButtonPressed(int8_t buttonIndex)
{
    if (!listeningForPlayerInput || ledBrightnessAdjustTimer > 0 || !Output::getLedStatus(buttonIndex))
    {
        return;
    }

    if (simonLedSequence[sequenceIndex] != buttonIndex)
    {
        --lives;

        LCD::Instance().writeByte(1, 12 - lives, 0x20);
        AudioSource::playNote(MusicNote::E1, 200);

        if (lives != 0)
        {
            listeningForPlayerInput = false;
            ledBrightnessAdjustTimer = 700;
            sequenceIndex = 0;
            return;
        }

        nextState = GameState::GameOver;
        return;
    }

    playNextSequenceNote();
}

void SimonState::playRoundSequence()
{
    uint8_t totalLedCount = GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 3 : 7;
    uint8_t randomLedIndex = Random::range(0, totalLedCount);

    ledBrightnessAdjustTimer = 500;
    sequenceIndex = 0;
    listeningForPlayerInput = false;
    simonLedSequence[currentRound] = ledOptions[randomLedIndex];
    simonMusicNoteSequence[currentRound] = musicNoteOptions[randomLedIndex];
    LCD::Instance().writeNumber(1, 2, ++currentRound);
}

void SimonState::listenForPlayerInput()
{
    ledBrightnessAdjustTimer = 0;
    sequenceIndex = 0;
    listeningForPlayerInput = true;
}

void SimonState::playNextSequenceNote()
{
    ledBrightnessAdjustTimer = 300;
    Output::setLedIntensity(simonLedSequence[sequenceIndex], .6);
    AudioSource::playNote(simonMusicNoteSequence[sequenceIndex], 200);
    activeLedIndex = simonLedSequence[sequenceIndex++];
}
