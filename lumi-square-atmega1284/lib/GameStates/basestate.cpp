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

    if (difficultyCount > 3)
    {
        difficultyCount = 1;
    }

    switch (difficultyCount)
    {
    case 1:
        difficulty = Difficulty::Easy;
        break;
    case 2:
        difficulty = Difficulty::Medium;
        break;
    case 3:
        difficulty = Difficulty::Hard;
        break;
    }
    gameDifficulty = difficulty;
    return difficulty;
}
