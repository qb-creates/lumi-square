#include "scoremanager.h"

ScoreManager::ScoreManager() : newHighScore(false), score(0) {}

ScoreManager &ScoreManager::Instance()
{
    static ScoreManager instance;
    return instance;
}

int16_t ScoreManager::getScore()
{
    return score;
}

void ScoreManager::addToScore(uint8_t value)
{
    score += value;
}

void ScoreManager::subtractFromScore(uint8_t value)
{
    score -= value;

    if (score < 0)
    {
        score = 0;
    }
}

void ScoreManager::resetScore()
{
    score = 0;
}
