#include "simonstate.h"
#include "ledmatrix.h"
#include "random.h"

SimonState::SimonState()
    : GameBaseState(GameState::Simon),
      buttonMapArray{5, 6, 9, 10, 0, 3, 12, 15},
      musicNoteMapArray{MusicNote::G4, MusicNote::C4, MusicNote::E4, MusicNote::G3, MusicNote::A4, MusicNote::D4, MusicNote::F4, MusicNote::B3},
      sequenceIndex(0),
      activeButtonIndex(0),
      currentRound(0),
      simonButtonSequence{},
      simonMusicNoteSequence{},
      listeningForPlayerInput(false),
      timer(30),
      highScoreAddress((uint8_t *)0) {}

void SimonState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);
    highScoreAddress = (uint8_t *)static_cast<int16_t>(GameProperties::Instance().gameDifficulty) + 6;
    lives = GameProperties::Instance().gameDifficulty == Difficulty::Hard ? 1 : 3;
    ScoreManager::Instance().resetScore(currentState, GameProperties::Instance().gameDifficulty);
    startNextRoundSequence();
}

void SimonState::exitState()
{
    currentRound = 0;
    sequenceIndex = 0;
    nextState = GameState::None;
    LEDMatrix::ledOn(buttonMapArray, 8, Colors::red, .8);
}

void SimonState::updateState()
{
    if (lives == 0)
    {
        nextState = GameState::GameOver;
        saveHighScore();
        return;
    }

    if (AudioSource::Instance().isMusicNoteSequencePlaying())
        return;

    // Subtract 16 milliseconds from the delay timer if it is greater than 0
    uint8_t time = !timer.isComplete() ? DeviceUtility::DELTA_TIME : 0;
    timer.updateTimer(time);

    if (!timer.isComplete())
        return;

    // Deactive the button led if it is active
    if (LEDMatrix::getLedIntensity(activeButtonIndex) == 1)
    {
        timer.setTargetTime(200);
        LEDMatrix::setLedIntensity(activeButtonIndex, .2);

        if (sequenceIndex == currentRound)
        {
            listeningForPlayerInput ? startNextRoundSequence() : listenForPlayerInput();
        }

        return;
    }

    if (!listeningForPlayerInput)
    {
        playNextSequenceElement();
    }
}

void SimonState::onButtonPressed(int8_t buttonIndex)
{
    // Return if not currently listening for player input, the selected button is off, or a button is active (active == intensity of 1)
    if (!listeningForPlayerInput || LEDMatrix::getLedIntensity(activeButtonIndex) == 1 || !LEDMatrix::getLedStatus(buttonIndex))
        return;

    if (simonButtonSequence[sequenceIndex] == buttonIndex)
    {
        playNextSequenceElement();
        return;
    }

    removeLifePoint();
    replayRoundSequence();
}

void SimonState::startNextRoundSequence()
{
    while (1)
    {
        uint8_t totalLedCount = GameProperties::Instance().gameDifficulty == Difficulty::Easy ? 3 : 7;
        uint8_t randomIndex = Random::range(0, totalLedCount);

        if (currentRound >= 2)
        {
            // Continue and grab another random button index if current random button is the same as the previous two buttons in the simon sequence.
            if (simonButtonSequence[currentRound - 1] == buttonMapArray[randomIndex] && simonButtonSequence[currentRound - 2] == buttonMapArray[randomIndex])
                continue;
        }

        timer.setTargetTime(500);
        sequenceIndex = 0;
        listeningForPlayerInput = false;
        simonButtonSequence[currentRound] = buttonMapArray[randomIndex];
        simonMusicNoteSequence[currentRound] = musicNoteMapArray[randomIndex];
        ++currentRound;
        break;
    }
}

void SimonState::replayRoundSequence()
{
    timer.setTargetTime(700);
    sequenceIndex = 0;
    listeningForPlayerInput = false;
}

void SimonState::listenForPlayerInput()
{
    timer.setTargetTime(0);
    sequenceIndex = 0;
    listeningForPlayerInput = true;
}

void SimonState::playNextSequenceElement()
{
    timer.setTargetTime(300);
    activeButtonIndex = simonButtonSequence[sequenceIndex];
    LEDMatrix::setLedIntensity(simonButtonSequence[sequenceIndex], 1);
    AudioSource::Instance().playMusicNote(simonMusicNoteSequence[sequenceIndex], 200);
    ++sequenceIndex;
}

void SimonState::removeLifePoint()
{
    --lives;
    AudioSource::Instance().playNoteSequence(FAILURE_AUDIO_CLIP);
}

void SimonState::saveHighScore()
{
    uint8_t highscore = StorageService::getHighScore(currentState, GameProperties::Instance().gameDifficulty);

    if (highscore < currentRound)
    {
        StorageService::saveHighScore(currentState, GameProperties::Instance().gameDifficulty, currentRound);
    }
}
