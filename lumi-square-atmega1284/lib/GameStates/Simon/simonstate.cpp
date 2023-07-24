#include "simonstate.h"
#include "leds.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(), pattern{}, patternNotes{}, patternIndex(-1), playPattern(0), isPlayingSequence(false), timer(0), buttonLedIsOn(false), gameOver(false) {}

void SimonState::enterState()
{
    for (int i = 0; i < 16; i++)
    {
        Output::setLedIntensity(i, .6);
    }
    Output::setLedColor(0, Colors::red);
    Output::setLedColor(3, Colors::red);
    Output::setLedColor(12, Colors::red);
    Output::setLedColor(15, Colors::red);
    Output::setLedColor(5, Colors::green);
    Output::setLedColor(6, Colors::red);
    Output::setLedColor(9, Colors::yellow);
    Output::setLedColor(10, Colors::azure);

    playNewSequence();
}

void SimonState::exitState()
{
    Output::setLedColor(0, Colors::red);
    Output::ledOn(0);

    Output::setLedColor(3, Colors::red);
    Output::ledOn(3);

    Output::setLedColor(5, Colors::red);
    Output::ledOn(5);

    Output::setLedColor(6, Colors::red);
    Output::ledOn(6);

    Output::setLedColor(9, Colors::red);
    Output::ledOn(9);

    Output::setLedColor(10, Colors::red);
    Output::ledOn(10);

    Output::setLedColor(12, Colors::red);
    Output::ledOn(12);

    Output::setLedColor(15, Colors::red);
    Output::ledOn(15);

    patternIndex = -1;
    playPattern = 0;
    isPlayingSequence = false;
    timer = 0;
    buttonLedIsOn = false;
    gameOver = false;
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
    if (!isPlayingSequence && !gameOver && !buttonLedIsOn)
    {
        switch (buttonIndex)
        {
        case 5:
            Output::setLedColor(buttonIndex, Colors::green);
            AudioSource::playNote(MusicNote::G5, 200);
            break;
        case 6:
            Output::setLedColor(buttonIndex, Colors::red);
            AudioSource::playNote(MusicNote::C5, 200);
            break;
        case 9:
            Output::setLedColor(buttonIndex, Colors::yellow);
            AudioSource::playNote(MusicNote::E5, 200);
            break;
        case 10:
            Output::setLedColor(buttonIndex, Colors::azure);
            AudioSource::playNote(MusicNote::G4, 200);
            break;
        default:
            return;
        }

        if (pattern[playPattern] == buttonIndex)
        {
            playPattern++;
            buttonLedIsOn = true;
        }
        else
        {
            gameOver = true;
            nextState = GameState::GameOver;
        }

        Output::ledOn(buttonIndex);
        timer = 300;
    }
}

void SimonState::playNewSequence()
{
    ++patternIndex;

    int randomButton = Random::range(1, 4);

    switch (randomButton)
    {
    case 1:
        pattern[patternIndex] = 5;
        patternNotes[patternIndex] = MusicNote::G5;
        break;
    case 2:
        pattern[patternIndex] = 6;
        patternNotes[patternIndex] = MusicNote::C5;
        break;
    case 3:
        pattern[patternIndex] = 9;
        patternNotes[patternIndex] = MusicNote::E5;
        break;
    case 4:
        pattern[patternIndex] = 10;
        patternNotes[patternIndex] = MusicNote::G4;
        break;
    }

    isPlayingSequence = true;
    timer = 700;
    playPattern = 0;
}
