#include "mainmenustate.h"
#include "buttons.h"
#include "leds.h"
#include "voiceover.h"
#include "voiceovermanager.h"
#include <random.h>

MainMenuState::MainMenuState()
    : GameBaseState(GameState::Menu),
      queuedState(GameState::None),
      maxDifficulty(Difficulty::Easy),
      countdownTimer(1000),
      countdownCounter(3),
      isStartingGame(false),
      gameIndex(0),
      firstEntry(true) { }

void MainMenuState::enterState()
{
    Output::ledOn(0);
    Output::setLedColor(0, Colors::red, 1);
    queueGameState(gameIndex, firstEntry);
    firstEntry = false;
}

void MainMenuState::exitState()
{
    countdownTimer = 1000;
    countdownCounter = 3;
    isStartingGame = false;
    nextState = GameState::None;
}

void MainMenuState::updateState()
{
    if (!isStartingGame)
    {
        if (Input::getDifficultyButtonUp() && !AudioSource::Instance().isMuteButtonsPressed())
        {
            increaseDifficulty();
        }

        if (Input::getPreviousButtonDown())
        {
            --gameIndex;

            if (gameIndex < 0)
            {
                gameIndex = 2;
            }

            queueGameState(gameIndex, true);
            AudioSource::Instance().playMusicNote(MusicNote::Ab3, 50);
        }

        if (Input::getNextButtonDown())
        {
            ++gameIndex;

            if (gameIndex > 2)
            {
                gameIndex = 0;
            }

            queueGameState(gameIndex, true);
            AudioSource::Instance().playMusicNote(MusicNote::Ab3, 50);
        }
    }

    updateCountdownTimer();
}

void MainMenuState::onButtonPressed(int8_t buttonIndex)
{
    if (!isStartingGame)
    {
        if (queuedState == GameState::Simon)
        {
            Output::enableSimonLights(GameProperties::Instance().gameDifficulty != Difficulty::Easy);
        }

        startCountdownTimer();
    }
}

void MainMenuState::queueGameState(int8_t gameIndex, bool playAudio)
{
    switch (gameIndex)
    {
    case 0:
        queuedState = GameState::MemoryMatching;
        this->maxDifficulty = Difficulty::Easy;
        if (playAudio) VoiceOverManager::PlayVoiceOver(VoiceOver::MemoryMatching);
        break;
    case 1:
        queuedState = GameState::Simon;
        this->maxDifficulty = Difficulty::Hard;
        if (playAudio) VoiceOverManager::PlayVoiceOver(VoiceOver::Simon);
        break;
    case 2:
        queuedState = GameState::LightDash;
        this->maxDifficulty = Difficulty::Hard;
        if (playAudio) VoiceOverManager::PlayVoiceOver(VoiceOver::LightDash);
        break;
    }

    GameProperties::Instance().setDifficulty(Difficulty::Easy);
}

void MainMenuState::increaseDifficulty()
{
    switch (GameProperties::Instance().increaseDifficulty())
    {
    case Difficulty::Easy:
        VoiceOverManager::PlayVoiceOver(VoiceOver::Easy);
        break;
    case Difficulty::Medium:
        VoiceOverManager::PlayVoiceOver(VoiceOver::Medium);
        break;
    case Difficulty::Hard:
        VoiceOverManager::PlayVoiceOver(VoiceOver::Hard);
        break;
    }
}

void MainMenuState::startCountdownTimer()
{
    isStartingGame = true;
    AudioSource::Instance().playMusicNote(MusicNote::C5, 200);
}

void MainMenuState::updateCountdownTimer()
{
    countdownTimer -= isStartingGame ? FixedUpdateTimer::DELTA_TIME : 0;

    if (countdownTimer <= 0)
    {
        --countdownCounter;
        countdownTimer = 1000;

        if (countdownCounter < 0)
        {
            Random::seedRNG();
            nextState = queuedState;
            return;
        }

        AudioSource::Instance().playMusicNote(countdownCounter == 0 ? MusicNote::E5 : MusicNote::C5, 200);
    }
}
