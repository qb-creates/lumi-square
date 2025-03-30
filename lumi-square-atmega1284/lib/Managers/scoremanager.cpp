#include "scoremanager.h"

int16_t ScoreManager::score = 0;
bool ScoreManager::newHighScore = false;

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
