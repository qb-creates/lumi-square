#include "lightspeedstate.h"
#include "audio.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

LightSpeedState::LightSpeedState()
    : GameBaseState(), timer(1000), counter(30), score(0), shuffleLeds(false), timePowerUpActive(false), redHazardActive(false), ledShuffleTimes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

void LightSpeedState::enterState()
{
    uint8_t i = 0;

    while (i < 5)
    {
        int ledIndex = Random::range(0, 15);

        if (Output::getLedStatus(ledIndex))
            continue;

        Output::setLedColor(ledIndex, Colors::azure, .5);
        Output::ledOn(ledIndex);
        ledShuffleTimes[ledIndex] = Random::range(500, 1500 );
        ++i;
    }

    if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
    {
        enablePowerUps = true;
    }
    shuffleLeds = true;

    LCD::Instance().writeString(0, 0, "  Score   Time  ");
    LCD::Instance().writeString(1, 0, "     0     30   ");
}

void LightSpeedState::exitState()
{
    score = 0;
    timer = 1000;
    counter = 30;
    nextState = GameState::None;
}

void LightSpeedState::updateState()
{
    if (shuffleLeds)
    {
        for (int ledIndex = 0; ledIndex < 16; ++ledIndex)
        {
            if (!Output::getLedStatus(ledIndex))
            {
                continue;
            }

            ledShuffleTimes[ledIndex] -= 16;

            if (ledShuffleTimes[ledIndex] <= 0)
            {
                powerOnRandomLight();
                
                if (Output::getLedColor(ledIndex) == Colors::green)
                {
                    timePowerUpActive = false;
                }

                if (Output::getLedColor(ledIndex) == Colors::red)
                {
                    redHazardActive = false;
                }

                Output::ledOff(ledIndex);
            }
        }
    }

    timer -= 16;

    if (timer <= 0)
    {
        timer = 1000;
        --counter;

        LCD::Instance().writeNumber(1, 10, counter);

        if (counter <= 0)
        {
            nextState = GameState::GameOver;
        }
    }
}

void LightSpeedState::onButtonPressed(int8_t buttonIndex)
{
    if (!Output::getLedStatus(buttonIndex))
        return;

    powerOnRandomLight();

    Color buttonColor = Output::getLedColor(buttonIndex);

    if (buttonColor == Colors::azure)
    {
        ++score;
        AudioSource::playNote(MusicNote::G5, 50);
    }
    else if (buttonColor == Colors::green)
    {
        counter += 7;
        timePowerUpActive = false;
        AudioSource::playNote(MusicNote::A5, 50);
    }
    else if (buttonColor == Colors::red)
    {
        if (GameProperties::Instance().gameDifficulty == Difficulty::Hard)
        {
            --score;
        }
        counter -= 3;
        redHazardActive = false;
        AudioSource::playNote(MusicNote::G6, 50);
    }

    Output::ledOff(buttonIndex);
    LCD::Instance().writeNumber(1, 3, score);
}

void LightSpeedState::powerOnRandomLight()
{
    while (1)
    {
        int ledIndex = Random::range(0, 15);

        if (!Output::getLedStatus(ledIndex))
        {
            Color ledColor = Colors::azure;

            if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
            {
                int asdf = Random::range(1, 3);

                if (!timePowerUpActive && asdf == 1)
                {
                    ledColor = Colors::green;
                    timePowerUpActive = true;
                }
                else if (!redHazardActive && asdf == 2)
                {
                    ledColor = Colors::red;
                    redHazardActive = true;
                }                
            }

            int16_t shuffleTime = ledColor != Colors::azure ?  1000 : Random::range(500, 1500 );
            Output::setLedColor(ledIndex, ledColor, .5);
            Output::ledOn(ledIndex);
            ledShuffleTimes[ledIndex] = shuffleTime;
            break;
        }
    }
}