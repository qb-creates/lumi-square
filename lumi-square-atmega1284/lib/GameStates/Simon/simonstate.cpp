#include "simonstate.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(), pattern{}, patternNotes{}, patternIndex(-1), playPattern(0), isPlayingSequence(false), timer(0), buttonLedIsOn(false) {}

void SimonState::enterState()
{
    lives = GameProperties::Instance().gameDifficulty == Difficulty::Hard ? 1 : 3;

    Output::setLedColor(0, Colors::aquamarine, .6);
    Output::setLedColor(3, Colors::pink, .6);
    Output::setLedColor(12, Colors::purple, .6);
    Output::setLedColor(15, Colors::cyan, .6);
    Output::setLedColor(5, Colors::green, .6);
    Output::setLedColor(6, Colors::red, .6);
    Output::setLedColor(9, Colors::yellow, .6);
    Output::setLedColor(10, Colors::azure, .6);

    LCD::Instance().writeString(0, 0, " Round    Lives ");
    LCD::Instance().writeString(1, 0, "    1           ");

    for (int i = 0; i < lives; ++i)
    {
        LCD::Instance().writeByte(1, 13 - i, 0x00);
    }

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
                    Output::ledOn(pattern[playPattern]);
                    timer = 300;
                    buttonLedIsOn = true;
                    playPattern++;
                }
                else if (buttonLedIsOn)
                {
                    timer = 200;
                    buttonLedIsOn = false;
                    Output::ledOff(pattern[playPattern - 1]);
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
                Output::ledOff(pattern[playPattern - 1]);

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
    if (!isPlayingSequence && !buttonLedIsOn && nextState != GameState::GameOver)
    {
        switch (buttonIndex)
        {
        case 1:
        case 2:
        case 4:
        case 7:
        case 8:
        case 11:
        case 13:
        case 14:
            return;
        case 5:
            AudioSource::playNote(MusicNote::G3, 200);
            break;
        case 6:
            AudioSource::playNote(MusicNote::C3, 200);
            break;
        case 9:
            AudioSource::playNote(MusicNote::E3, 200);
            break;
        case 10:
            AudioSource::playNote(MusicNote::G2, 200);
            break;
        }

        if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
        {
            switch (buttonIndex)
            {
            case 0:
                AudioSource::playNote(MusicNote::G4, 200);
                break;
            case 3:
                AudioSource::playNote(MusicNote::C4, 200);
                break;
            case 12:
                AudioSource::playNote(MusicNote::E4, 200);
                break;
            case 15:
                AudioSource::playNote(MusicNote::G3, 200);
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

        Output::ledOn(buttonIndex);
        timer = 300;
    }
}

void SimonState::playNewSequence()
{
    ++patternIndex;

    int randomButton = Random::range(1, GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 4 : 8);

    switch (randomButton)
    {
    case 1:
        pattern[patternIndex] = 5;
        patternNotes[patternIndex] = MusicNote::G3;
        break;
    case 2:
        pattern[patternIndex] = 6;
        patternNotes[patternIndex] = MusicNote::C3;
        break;
    case 3:
        pattern[patternIndex] = 9;
        patternNotes[patternIndex] = MusicNote::E3;
        break;
    case 4:
        pattern[patternIndex] = 10;
        patternNotes[patternIndex] = MusicNote::G2;
        break;
    case 5:
        pattern[patternIndex] = 0;
        patternNotes[patternIndex] = MusicNote::G4;
        break;
    case 6:
        pattern[patternIndex] = 3;
        patternNotes[patternIndex] = MusicNote::C4;
        break;
    case 7:
        pattern[patternIndex] = 12;
        patternNotes[patternIndex] = MusicNote::E4;
        break;
    case 8:
        pattern[patternIndex] = 15;
        patternNotes[patternIndex] = MusicNote::G3;
        break;
    }

    isPlayingSequence = true;
    timer = 700;
    playPattern = 0;

    LCD::Instance().writeNumber(1, 2, patternIndex + 1);
}

void SimonState::replaySequence()
{
}

void SimonState::playNote(int8_t buttonIndex)
{
    switch (buttonIndex)
    {
    case 5:
        AudioSource::playNote(MusicNote::G4, 200);
        break;
    case 6:
        AudioSource::playNote(MusicNote::C4, 200);
        break;
    case 9:
        AudioSource::playNote(MusicNote::E4, 200);
        break;
    case 10:
        AudioSource::playNote(MusicNote::G3, 200);
        break;
    }

    if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
    {
        switch (buttonIndex)
        {
        case 0:
            AudioSource::playNote(MusicNote::G5, 200);
            break;
        case 3:
            AudioSource::playNote(MusicNote::C5, 200);
            break;
        case 12:
            AudioSource::playNote(MusicNote::E5, 200);
            break;
        case 15:
            AudioSource::playNote(MusicNote::G4, 200);
            break;
        }
    }
}
