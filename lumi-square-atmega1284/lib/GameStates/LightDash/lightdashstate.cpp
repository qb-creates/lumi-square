#include "lightdashstate.h"
#include "leds.h"
#include "random.h"
#include "voiceovermanager.h"

LightDashState::LightDashState()
    : GameBaseState(GameState::LightDash),
      gameTimer(1000),
      remainingGameTime(30),
      scoreDeductionAmount(0),
      bonusLedPointValue(0),
      maxLedTurnOffTime(1500),
      minLedTurnOffTime(500),
      bonusLedTurnOffTime(500),
      bonusLedActive(false),
      ledTurnOffTimers{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

void LightDashState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);
    Difficulty difficulty = GameProperties::Instance().gameDifficulty;

    switch (difficulty)
    {
    case Difficulty::Easy:
        maxLedTurnOffTime = 2500;
        minLedTurnOffTime = 1500;
        break;
    case Difficulty::Medium:
        maxLedTurnOffTime = 2000;
        minLedTurnOffTime = 1000;
        break;
    case Difficulty::Hard:
        maxLedTurnOffTime = 1200;
        minLedTurnOffTime = 750;
        break;
    }

    scoreDeductionAmount = static_cast<uint8_t>(difficulty) + 1;
    bonusLedPointValue = difficulty != Difficulty::Medium ? 5 : 3;
    bonusLedTurnOffTime = difficulty != Difficulty::Medium ? 400 : 500;
    enablePowerUps = difficulty != Difficulty::Easy;

    uint8_t i = 0;

    while (i < 5)
    {
        int ledIndex = Random::range(0, 15);

        if (Output::getLedStatus(ledIndex))
            continue;

        Output::setLedColor(ledIndex, Colors::azure, .8);
        Output::ledOn(ledIndex);
        ledTurnOffTimers[ledIndex] = Random::range(minLedTurnOffTime, maxLedTurnOffTime);
        ++i;
    }
}

void LightDashState::exitState()
{
    gameTimer = 1000;
    remainingGameTime = 30;
    nextState = GameState::None;
}

void LightDashState::updateState()
{
    updateLedOffTimers();
    updateGameTimer();
}

void LightDashState::onButtonPressed(int8_t buttonIndex)
{
    if (!Output::getLedStatus(buttonIndex))
    {
        deductPointsFromScore();
        return;
    }

    turnOnRandomLed();
    turnOffSelectedLed(buttonIndex);
    playMusicNote(buttonIndex);
    addPointsToScore(buttonIndex);
}

void LightDashState::updateLedOffTimers()
{
    for (int ledIndex = 0; ledIndex < 16; ++ledIndex)
    {
        if (!Output::getLedStatus(ledIndex))
        {
            continue;
        }

        ledTurnOffTimers[ledIndex] -= FixedUpdateTimer::DELTA_TIME;

        if (ledTurnOffTimers[ledIndex] <= 0)
        {
            turnOnRandomLed();

            if (Output::getLedColor(ledIndex) == Colors::green)
            {
                bonusLedActive = false;
            }

            Output::ledOff(ledIndex);
        }
    }
}

void LightDashState::updateGameTimer()
{
    gameTimer -= FixedUpdateTimer::DELTA_TIME;

    if (gameTimer > 0)
        return;

    --remainingGameTime;

    if (remainingGameTime == 0)
        nextState = GameState::GameOver;

    gameTimer = 1000;
}

void LightDashState::turnOnRandomLed()
{
    while (1)
    {
        int ledIndex = Random::range(0, 15);
        Color ledColor = Colors::azure;

        if (Output::getLedStatus(ledIndex))
            continue;

        if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
        {
            int randomNumber = Random::range(1, 20);

            if (!bonusLedActive && randomNumber == 5)
            {
                ledColor = Colors::green;
                bonusLedActive = true;
            }
        }

        int16_t turnOffTime = ledColor != Colors::azure ? bonusLedTurnOffTime : Random::range(minLedTurnOffTime, maxLedTurnOffTime);
        ledTurnOffTimers[ledIndex] = turnOffTime;

        Output::setLedColor(ledIndex, ledColor, .8);
        Output::ledOn(ledIndex);
        break;
    }
}

void LightDashState::turnOffSelectedLed(int8_t buttonIndex)
{
    Color buttonColor = Output::getLedColor(buttonIndex);

    if (buttonColor == Colors::green)
    {
        bonusLedActive = false;
    }

    Output::ledOff(buttonIndex);
}

void LightDashState::deductPointsFromScore()
{
    ScoreManager::subtractFromScore(scoreDeductionAmount);
}

void LightDashState::addPointsToScore(int8_t buttonIndex)
{
    Color buttonColor = Output::getLedColor(buttonIndex);
    int points = buttonColor == Colors::azure ? 1 : bonusLedPointValue;
    ScoreManager::addToScore(points);
}

void LightDashState::playMusicNote(int8_t buttonIndex)
{
    Color buttonColor = Output::getLedColor(buttonIndex);
    MusicNote musicNote = buttonColor == Colors::azure ? MusicNote::G5 : MusicNote::A5;
    AudioSource::Instance().playMusicNote(musicNote, 50);
}
