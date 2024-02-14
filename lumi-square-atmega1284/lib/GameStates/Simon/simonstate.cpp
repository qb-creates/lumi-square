#include "simonstate.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(),
      pattern{},
      buttonOptions{5, 6, 9, 10, 0, 3, 12, 15},
      musicNoteOptions{MusicNote::G3, MusicNote::C3, MusicNote::E3, MusicNote::G2, MusicNote::G4, MusicNote::C4, MusicNote::E4, MusicNote::G3},
      patternNotes{},
      patternIndex(-1),
      playPattern(0),
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

    patternIndex = -1;
    playPattern = 0;
    isPlayingSequence = false;
    timer = 0;
    lives = 0;
    buttonLedIsOn = false;
    nextState = GameState::None;
}

void SimonState::updateState()
{
    if (timer > 0)
    {
        timer -= 16;

        if (timer <= 0)
        {
            if (isPlayingSequence)
            {
                if (playPattern <= patternIndex && !buttonLedIsOn)
                {
                    AudioSource::playNote(patternNotes[playPattern], 200);
                    Output::setLedIntensity(pattern[playPattern], .6);
                    timer = 300;
                    buttonLedIsOn = true;
                    playPattern++;
                }
                else if (buttonLedIsOn)
                {
                    timer = 200;
                    buttonLedIsOn = false;
                    Output::setLedIntensity(pattern[playPattern - 1], .1);
                }
                else if (playPattern > patternIndex)
                {
                    isPlayingSequence = false;
                    playPattern = 0;
                }
            }
            else
            {
                buttonLedIsOn = false;
                Output::setLedIntensity(pattern[playPattern - 1], .1);

                if (playPattern > patternIndex)
                {
                    playNewSequence();
                }
            }
        }
    }
}

void SimonState::onButtonPressed(int8_t buttonIndex)
{
    if (!isPlayingSequence && !buttonLedIsOn && nextState != GameState::GameOver && Output::getLedStatus(buttonIndex))
    {
        int maxIndex = GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 4 : 8;

        for (int i = 0; i < maxIndex; ++i)
        {
            if (buttonOptions[i] == buttonIndex)
            {
                AudioSource::playNote(musicNoteOptions[i], 200);
                break;
            }
        }

        if (pattern[playPattern] == buttonIndex)
        {
            playPattern++;
            buttonLedIsOn = true;
        }
        else
        {
            --lives;

            LCD::Instance().writeByte(1, 13 - lives, 0x20);

            if (lives == 0)
            {
                nextState = GameState::GameOver;
            }
            else
            {
                isPlayingSequence = true;
                timer = 700;
                playPattern = 0;
                return;
            }
        }

        Output::setLedIntensity(buttonIndex, .6);
        timer = 300;
    }
}

void SimonState::playNewSequence()
{
    ++patternIndex;

    int randomButton = Random::range(0, GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 3 : 7);

    pattern[patternIndex] = buttonOptions[randomButton];
    patternNotes[patternIndex] = musicNoteOptions[randomButton];
    isPlayingSequence = true;
    timer = 700;
    playPattern = 0;

    LCD::Instance().writeNumber(1, 2, patternIndex + 1);
}