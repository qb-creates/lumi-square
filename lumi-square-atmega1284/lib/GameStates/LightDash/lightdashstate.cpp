#include "lightdashstate.h"
#include "lcd.h"
#include "leds.h"
#include "random.h"

LightDashState::LightDashState()
    : GameBaseState(GameState::LightDash),
      timer(1000),
      counter(30),
      score(0),
      shuffleLeds(false),
      timePowerUpActive(false),
      ledShuffleTimes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

void LightDashState::enterState()
{
    uint8_t i = 0;

    while (i < 5)
    {
        int ledIndex = Random::range(0, 15);

        if (Output::getLedStatus(ledIndex))
            continue;

        Output::setLedColor(ledIndex, Colors::azure, .5);
        Output::ledOn(ledIndex);
        ledShuffleTimes[ledIndex] = Random::range(500, 1500);
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

void LightDashState::exitState()
{
    score = 0;
    timer = 1000;
    counter = 30;
    nextState = GameState::None;
}

void LightDashState::updateState()
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

void LightDashState::onButtonPressed(int8_t buttonIndex)
{
    if (!Output::getLedStatus(buttonIndex))
    {
        --score;
        LCD::Instance().writeNumber(1, 3, score);
        return;
    }

    powerOnRandomLight();

    Color buttonColor = Output::getLedColor(buttonIndex);
    int points = buttonColor == Colors::azure ? 1 : 3;
    MusicNote musicNote = buttonColor == Colors::azure ? MusicNote::G5 : MusicNote::A5;

    if (buttonColor == Colors::green)
    {
        timePowerUpActive = false;
    }

    score += points;
    AudioSource::playMusicNote(musicNote, 50);

    Output::ledOff(buttonIndex);
    LCD::Instance().writeNumber(1, 3, score);
}

void LightDashState::powerOnRandomLight()
{
    while (1)
    {
        int ledIndex = Random::range(0, 15);

        if (!Output::getLedStatus(ledIndex))
        {
            Color ledColor = Colors::azure;

            if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
            {
                int asdf = Random::range(1, 20);

                if (!timePowerUpActive && asdf == 5)
                {
                    ledColor = Colors::green;
                    timePowerUpActive = true;
                }
            }

            int16_t shuffleTime = ledColor != Colors::azure ? 500 : Random::range(500, 1500);
            Output::setLedColor(ledIndex, ledColor, .5);
            Output::ledOn(ledIndex);
            ledShuffleTimes[ledIndex] = shuffleTime;
            break;
        }
    }
}