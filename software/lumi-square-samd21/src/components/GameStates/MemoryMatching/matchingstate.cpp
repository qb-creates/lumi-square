#include "./matchingstate.h"
#include "../../Peripherals/ledmatrix.h"
#include "../../Utilities/random.h"

MemoryMatchingState::MemoryMatchingState()
    : GameBaseState(GameState::MemoryMatching),
      ledBrightnessAdjustTimer(0),
      correctMatches(0),
      selectedLedIndex1(-1),
      selectedLedIndex2(-1),
      isGuessCorrect(false),
      colorList {
          Colors::pink,
          Colors::turquoise,
          Colors::blue,
          Colors::purple,
          Colors::green,
          Colors::red,
          Colors::orange,
          Colors::yellow,
          Colors::green,
          Colors::red,
          Colors::turquoise,
          Colors::purple,
          Colors::pink,
          Colors::blue,
          Colors::orange,
          Colors::yellow} {}

void MemoryMatchingState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);
    shuffleLedColors();
}

void MemoryMatchingState::exitState()
{
    ledBrightnessAdjustTimer.setTargetTime(0);
    correctMatches = 0;
    selectedLedIndex1 = -1;
    selectedLedIndex2 = -1;
    isGuessCorrect = false;
    nextState = GameState::None;
}

void MemoryMatchingState::updateState()
{
    if (ledBrightnessAdjustTimer.isComplete())
        return;

    ledBrightnessAdjustTimer.updateTimer(DeviceUtility::DELTA_TIME);

    if (!ledBrightnessAdjustTimer.isComplete())
        return;

    adjustLedBrightness();

    if (correctMatches == 8)
    {
        nextState = GameState::GameOver;
    }
}

void MemoryMatchingState::onButtonPressed(int8_t buttonIndex)
{
    if (!ledBrightnessAdjustTimer.isComplete() || LEDMatrix::getLedStatus(buttonIndex))
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
    LEDMatrix::ledOn(buttonIndex);
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
        LEDMatrix::setLedColor(i, colorList[i], 1);
    }
}

void MemoryMatchingState::evaluateGuess()
{
    if (LEDMatrix::getLedColor(selectedLedIndex1) == LEDMatrix::getLedColor(selectedLedIndex2))
    {
        isGuessCorrect = true;
        ledBrightnessAdjustTimer.setTargetTime(300);
    }
    else
    {
        isGuessCorrect = false;
        ledBrightnessAdjustTimer.setTargetTime(600);
    }
}

void MemoryMatchingState::adjustLedBrightness()
{
    if (isGuessCorrect)
    {
        LEDMatrix::setLedIntensity(selectedLedIndex1, .4);
        LEDMatrix::setLedIntensity(selectedLedIndex2, .4);
        correctMatches++;
    }
    else
    {
        LEDMatrix::ledOff(selectedLedIndex1);
        LEDMatrix::ledOff(selectedLedIndex2);
    }

    selectedLedIndex1 = -1;
    selectedLedIndex2 = -1;
}
