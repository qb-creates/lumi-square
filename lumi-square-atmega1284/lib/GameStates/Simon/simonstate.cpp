#include "simonstate.h"
#include "lcd.h"
#include "leds.h"
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
      delayTimer(0),
      highScoreAddress((uint8_t *)0) {}

void SimonState::enterState()
{
    if (GameProperties::Instance().gameDifficulty != Difficulty::Easy)
    {
        Output::ledOn(0, Colors::aquamarine, .1);
        Output::ledOn(3, Colors::pink, .1);
        Output::ledOn(12, Colors::purple, .1);
        Output::ledOn(15, Colors::orange, .1);
    }

    Output::ledOn(5, Colors::green, .1);
    Output::ledOn(6, Colors::red, .1);
    Output::ledOn(9, Colors::yellow, .1);
    Output::ledOn(10, Colors::azure, .1);
    LCD::Instance().writeString(0, 0, " Round  Lives  \x02");
    LCD::Instance().writeString(1, 0, "    1           ");

    highScoreAddress = (uint8_t *)static_cast<int16_t>(GameProperties::Instance().gameDifficulty) + 6;
    lives = GameProperties::Instance().gameDifficulty == Difficulty::Hard ? 1 : 3;

    for (int i = 0; i < lives; ++i)
    {
        LCD::Instance().writeByte(1, 11 - i, 0x01);
    }

    startNextRoundSequence();
}

void SimonState::exitState()
{
    currentRound = 0;
    sequenceIndex = 0;
    nextState = GameState::None;
    Output::ledOn(buttonMapArray, 8, Colors::red, .6);
}

void SimonState::updateState()
{
    if (lives == 0)
    {
        nextState = GameState::GameOver;
        saveHighScore();
        return;
    }

    if (AudioSource::isPlayingAudioClip())
        return;
        
    // Subtract 16 milliseconds from the delay timer if it is greater than 0
    delayTimer -= (delayTimer > 0) ? 16 : 0;

    if (delayTimer > 0)
        return;

    // Deactive the button led if it is active
    if (Output::getLedIntensity(activeButtonIndex) == .6)
    {
        delayTimer = 200;
        Output::setLedIntensity(activeButtonIndex, .1);

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
    // Return if not currently listening for player input, the selected button is off, or a button is active (active == intensity of .6)
    if (!listeningForPlayerInput || Output::getLedIntensity(activeButtonIndex) == .6 || !Output::getLedStatus(buttonIndex))
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

        delayTimer = 500;
        sequenceIndex = 0;
        listeningForPlayerInput = false;
        simonButtonSequence[currentRound] = buttonMapArray[randomIndex];
        simonMusicNoteSequence[currentRound] = musicNoteMapArray[randomIndex];
        LCD::Instance().writeNumber(1, 2, ++currentRound);
        break;
    }
}

void SimonState::replayRoundSequence()
{
    delayTimer = 700;
    sequenceIndex = 0;
    listeningForPlayerInput = false;
}

void SimonState::listenForPlayerInput()
{
    delayTimer = 0;
    sequenceIndex = 0;
    listeningForPlayerInput = true;
}

void SimonState::playNextSequenceElement()
{
    delayTimer = 300;
    activeButtonIndex = simonButtonSequence[sequenceIndex];
    Output::setLedIntensity(simonButtonSequence[sequenceIndex], .6);
    AudioSource::playMusicNote(simonMusicNoteSequence[sequenceIndex], 200);
    ++sequenceIndex;
}

void SimonState::removeLifePoint()
{
    --lives;
    LCD::Instance().writeByte(1, (11 - lives), 0x20);
    AudioSource::playAudioClip(FAILURE_AUDIO_CLIP);
}

void SimonState::saveHighScore()
{
    uint8_t highscore = HighScoreManager::getHighScore(currentState, GameProperties::Instance().gameDifficulty);

    if (highscore < currentRound)
    {
        HighScoreManager::saveHighScore(currentState, GameProperties::Instance().gameDifficulty, currentRound);
    }
}
