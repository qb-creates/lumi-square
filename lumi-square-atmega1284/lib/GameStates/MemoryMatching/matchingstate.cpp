#include "./matchingstate.h"
#include "../../Peripherals/leds.h"
#include "../../STD/random.h"

MemoryMatchingState::MemoryMatchingState()
    : GameBaseState(GameState::MemoryMatching),
      ledBrightnessAdjustTimer(0),
      correctMatches(0),
      selectedLedIndex1(-1),
      selectedLedIndex2(-1),
      isGuessCorrect(false),
      colorList {
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
          Colors::yellow} {}

void MemoryMatchingState::enterState()
{
    shuffleLedColors();
}

void MemoryMatchingState::exitState()
{
    ledBrightnessAdjustTimer = 0;
    correctMatches = 0;
    selectedLedIndex1 = -1;
    selectedLedIndex2 = -1;
    isGuessCorrect = false;
    nextState = GameState::None;
}

void MemoryMatchingState::updateState()
{
    if (ledBrightnessAdjustTimer <= 0)
        return;

    ledBrightnessAdjustTimer -= 16;

    if (ledBrightnessAdjustTimer >= 0)
        return;

    adjustLedBrightness();

    if (correctMatches == 8)
    {
        nextState = GameState::GameOver;
    }
}

void MemoryMatchingState::onButtonPressed(int8_t buttonIndex)
{
    if (ledBrightnessAdjustTimer > 0 || Output::getLedStatus(buttonIndex))
        return;

    if (selectedLedIndex1 == -1)
    {
        selectedLedIndex1 = buttonIndex;
    }
    else
    {
        selectedLedIndex2 = buttonIndex;
        evaluateGuess();
    }

    MusicNote musicNote = selectedLedIndex2 == -1 ? MusicNote::C5 : MusicNote::D5;
    Output::ledOn(buttonIndex);
    AudioSource::Instance().playMusicNote(musicNote, 100);
}

void MemoryMatchingState::shuffleLedColors()
{
    // Shuffle the colorList array.
    for (int i = 15; i > 0; --i)
    {
        int j = Random::range(0, 15);
        Color temp = colorList[i];
        colorList[i] = colorList[j];
        colorList[j] = temp;
    }

    // Set Led colors.
    for (int i = 0; i < 16; ++i)
    {
        Output::setLedColor(i, colorList[i], 1);
    }
}

void MemoryMatchingState::evaluateGuess()
{
    if (Output::getLedColor(selectedLedIndex1) == Output::getLedColor(selectedLedIndex2))
    {
        isGuessCorrect = true;
        ledBrightnessAdjustTimer = 300;
    }
    else
    {
        isGuessCorrect = false;
        ledBrightnessAdjustTimer = 600;
    }
}

void MemoryMatchingState::adjustLedBrightness()
{
    if (isGuessCorrect)
    {
        Output::setLedIntensity(selectedLedIndex1, .4);
        Output::setLedIntensity(selectedLedIndex2, .4);
        correctMatches++;
    }
    else
    {
        Output::ledOff(selectedLedIndex1);
        Output::ledOff(selectedLedIndex2);
    }

    selectedLedIndex1 = -1;
    selectedLedIndex2 = -1;
}
