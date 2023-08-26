#include "lightspeedstate.h"
#include "audio.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"
LightSpeedState::LightSpeedState()
    : GameBaseState(), timer(1000), counter(30), score(0), lightCount(2000) {}

void LightSpeedState::enterState()
{
    for (int i = 0; i < 16; i++)
    {
        Output::setLedColor(i, Colors::azure, .6);
    }

    int temp[5] = {0, 0, 0, 0, 0};
    int tempAddress = 0;

    for (int i = 0; i < 5; i++)
    {
        while (1)
        {
            int value = Random::range(1, 16);
            bool valueRepeated = false;

            for (int i = 0; i < 5; i++)
            {
                if (value == temp[i])
                {
                    valueRepeated = true;
                    break;
                }
            }

            if (valueRepeated)
                continue;

            temp[tempAddress] = value;
            tempAddress++;

            Output::ledOn(value - 1);
            break;
        }
    }

    LCD::Instance().clearDisplay();
    LCD::Instance().writeChars(0, 0, "Light Speed     ");
    LCD::Instance().writeChars(1, 0, "Score:0    T:30");
}

void LightSpeedState::exitState()
{
    lightCount = 2000;
    score = 0;
    timer = 30000;
    nextState = GameState::None;
}

void LightSpeedState::updateState()
{
    lightCount -= 16;

    if (lightCount <= 0)
    {
        // int previous = 0;
        // while (1)
        // {
        //     int value = Random::range(1, 16);

        //     if (!Output::getLedStatus(value - 1) && (value - 1))
        //     {
        //         previous = value;
        //         Output::ledOn(value - 1);
        //         break;
        //     }
        // }

        // while (1)
        // {
        //     int value = Random::range(1, 16);

        //     if (Output::getLedStatus(value - 1) && previous != value)
        //     {
        //         Output::ledOff(value - 1);
        //         break;
        //     }
        // }

        lightCount = 1000;
    }
    timer -= 16;

    if (timer <= 0)
    {
        timer = 1000;
        --counter;

        LCD::Instance().writeInteger(1, 13, counter);
        if (counter <= 0)
        {
            nextState = GameState::GameOver;
        }
    }
    
}

void LightSpeedState::onButtonPressed(int8_t buttonIndex)
{
    if (Output::getLedStatus(buttonIndex))
    {
        while (1)
        {
            int value = Random::range(1, 16);

            if (!Output::getLedStatus(value - 1))
            {
                Output::ledOn(value - 1);
                break;
            }
        }

        Output::ledOff(buttonIndex);
        AudioSource::playNote(MusicNote::G6, 50);
        ++score;
    }
    else
    {
        score -= 2;

        if (score < 0)
        {
            score = 0;
        }
    }

    LCD::Instance().writeInteger(1, 6, score);
}
