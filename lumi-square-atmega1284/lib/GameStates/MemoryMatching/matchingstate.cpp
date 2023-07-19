#include "matchingstate.h"
#include "audio.h"
#include "leds.h"
#include "random.h"

MemoryMatchingState::MemoryMatchingState() : GameBaseState(), firstGuessIndex(-1), secondGuessIndex(-1), guessCorrect(false), timer(0), correctGueses(0) {}

void MemoryMatchingState::enterState()
{
    randomizeLights();
}

void MemoryMatchingState::exitState()
{
    firstGuessIndex = -1;
    secondGuessIndex = -1;
    guessCorrect = false;
    timer = 0;
    correctGueses = 0;
    nextState = GameState::None;
}

void MemoryMatchingState::updateState()
{
    if (timer >= 0)
    {
        timer -= 16;

        if (timer <= 0)
        {
            if (guessCorrect)
            {
                Output::setLedIntensity(firstGuessIndex, .2);
                Output::setLedIntensity(secondGuessIndex, .2);
                correctGueses++;
            }
            else
            {
                Output::ledOff(firstGuessIndex);
                Output::ledOff(secondGuessIndex);
            }
            firstGuessIndex = -1;
            secondGuessIndex = -1;
            guessCorrect = false;
        }
    }

    if (correctGueses == 8)
    {
        // if (Input::getButtonDown(1))
        // {
        //     resetGame();
        // }
        // if (Input::getButtonDown(0))
        // {
        //     returnToMainMenu();
        // }
    }
}

void MemoryMatchingState::onButtonPressed(int8_t buttonIndex)
{
    if (timer < 0 && !Output::getLedStatus(buttonIndex))
    {
        if (firstGuessIndex == -1)
        {
            firstGuessIndex = buttonIndex;
        }
        else if (secondGuessIndex == -1)
        {
            secondGuessIndex = buttonIndex;

            guessCorrect = (Output::getLedColor(firstGuessIndex) == Output::getLedColor(secondGuessIndex));
            timer = guessCorrect ? 300 : 600;
        }
        AudioSource::playNote(secondGuessIndex == -1 ? MusicNote::C6 : MusicNote::D6, 100);
        Output::ledOn(buttonIndex);
    }
}

void MemoryMatchingState::randomizeLights()
{
    static Color colorList[16] = {
        Colors::pink,
        Colors::azure,
        Colors::cyan,
        Colors::purple,
        Colors::aquamarine,
        Colors::red,
        Colors::orange,
        Colors::yellow,
        Colors::aquamarine,
        Colors::red,
        Colors::azure,
        Colors::purple,
        Colors::pink,
        Colors::cyan,
        Colors::orange,
        Colors::yellow};

    int temp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int tempAddress = 0;

    for (int i = 0; i < 16; i++)
    {
        Output::ledOff(i);
        Output::setLedIntensity(i, 1);

        while (1)
        {
            int value = Random::range(1, 16);
            bool valueRepeated = false;

            for (int i = 0; i < 16; i++)
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

            Output::setLedColor(i, colorList[(value - 1)]);
            break;
        }
    }
}
