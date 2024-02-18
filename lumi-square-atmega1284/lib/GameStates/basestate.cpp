#include "basestate.h"

GameBaseState::GameBaseState() : nextState(GameState::None) {}

GameState GameBaseState::switchStateCheck()
{
    return nextState;
}

GameProperties::GameProperties()
    : gameDifficulty(Difficulty::Easy), difficultyCount(1)
{
}

GameProperties &GameProperties::Instance()
{
    static GameProperties instance;
    return instance;
}

Difficulty GameProperties::increaseDifficulty()
{
    Difficulty difficulty = Difficulty::Easy;

    ++difficultyCount;

    if (difficultyCount > 2)
    {
        difficultyCount = 0;
    }

    switch (difficultyCount)
    {
    case 0:
        difficulty = Difficulty::Easy;
        break;
    case 1:
        difficulty = Difficulty::Medium;
        break;
    case 2:
        difficulty = Difficulty::Hard;
        break;
    }
    gameDifficulty = difficulty;
    return difficulty;
}

void GameProperties::setDifficulty(Difficulty difficulty)
{
    gameDifficulty = difficulty;
    difficultyCount = static_cast<int8_t>(difficulty);
}
