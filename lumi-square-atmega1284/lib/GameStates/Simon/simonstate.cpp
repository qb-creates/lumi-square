#include "simonstate.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(),
      simonSequenceIndex(0),
      playerSequenceIndex(-1),
      simonLightSequence{},
      simonMusicNoteSequence{},
      buttonOptions{5, 6, 9, 10, 0, 3, 12, 15},
      musicNoteOptions{MusicNote::G3, MusicNote::C3, MusicNote::E3, MusicNote::G2, MusicNote::G4, MusicNote::C4, MusicNote::E4, MusicNote::G3},
      isPlayingSequence(false),
      timer(0),
      buttonLedIsOn(false) {}

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

    LCD::Instance().writeString(0, 0, " Round    Lives ");
    LCD::Instance().writeString(1, 0, "    1      \x01\x01\x01  ");

    playNewSequence();
}

void SimonState::exitState()
{
    Output::ledOn(0, Colors::red, .6);
    Output::ledOn(3, Colors::red, .6);
    Output::ledOn(5, Colors::red, .6);
    Output::ledOn(6, Colors::red, .6);
    Output::ledOn(9, Colors::red, .6);
    Output::ledOn(10, Colors::red, .6);
    Output::ledOn(12, Colors::red, .6);
    Output::ledOn(15, Colors::red, .6);

    playerSequenceIndex = -1;
    simonSequenceIndex = 0;
    isPlayingSequence = false;
    timer = 0;
    lives = 0;
    buttonLedIsOn = false;
    nextState = GameState::None;
}

void SimonState::updateState()
{
    if (timer <= 0)
        return;

    timer -= 16;

    if (timer <= 0)
    {
        if (buttonLedIsOn)
        {
            timer = isPlayingSequence ? 200 : 0;
            buttonLedIsOn = false;
            Output::setLedIntensity(simonLightSequence[simonSequenceIndex - 1], .1);

            if (simonSequenceIndex > playerSequenceIndex && !isPlayingSequence)
            {
                playNewSequence();
            }

            return;
        }

        if (simonSequenceIndex <= playerSequenceIndex)
        {
            AudioSource::playNote(simonMusicNoteSequence[simonSequenceIndex], 200);
            Output::setLedIntensity(simonLightSequence[simonSequenceIndex], .6);
            timer = 300;
            buttonLedIsOn = true;
            simonSequenceIndex++;
            return;
        }

        isPlayingSequence = false;
        simonSequenceIndex = 0;
    }
}

void SimonState::onButtonPressed(int8_t buttonIndex)
{
    if (isPlayingSequence || buttonLedIsOn || !Output::getLedStatus(buttonIndex))
    {
        return;
    }

    int maxIndex = GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 4 : 8;

    for (int i = 0; i < maxIndex; ++i)
    {
        if (buttonOptions[i] == buttonIndex)
        {
            AudioSource::playNote(musicNoteOptions[i], 200);
            break;
        }
    }

    if (simonLightSequence[simonSequenceIndex] != buttonIndex)
    {
        --lives;

        LCD::Instance().writeByte(1, 13 - lives, 0x20);

        if (lives != 0)
        {
            isPlayingSequence = true;
            timer = 700;
            simonSequenceIndex = 0;
            return;
        }

        nextState = GameState::GameOver;
    }

    simonSequenceIndex++;
    buttonLedIsOn = true;

    Output::setLedIntensity(buttonIndex, .6);
    timer = 300;
}

void SimonState::playNewSequence()
{
    ++playerSequenceIndex;

    int randomButton = Random::range(0, GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 3 : 7);

    simonLightSequence[playerSequenceIndex] = buttonOptions[randomButton];
    simonMusicNoteSequence[playerSequenceIndex] = musicNoteOptions[randomButton];
    isPlayingSequence = true;
    timer = 700;
    simonSequenceIndex = 0;

    LCD::Instance().writeNumber(1, 2, playerSequenceIndex + 1);
}