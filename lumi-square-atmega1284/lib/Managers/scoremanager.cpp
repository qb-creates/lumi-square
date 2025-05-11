#include "scoremanager.h"

ScoreManager::ScoreManager() : newHighScore(false), score(0), highScoreMatrix{}, gameState(GameState::None)
{
    // Grab Matching High Scores
    highScoreMatrix[static_cast<uint8_t>(GameState::MemoryMatching)][static_cast<uint8_t>(Difficulty::Easy)] = StorageService::getHighScore(GameState::MemoryMatching, Difficulty::Easy);
    highScoreMatrix[static_cast<uint8_t>(GameState::MemoryMatching)][static_cast<uint8_t>(Difficulty::Medium)] = StorageService::getHighScore(GameState::MemoryMatching, Difficulty::Medium);
    highScoreMatrix[static_cast<uint8_t>(GameState::MemoryMatching)][static_cast<uint8_t>(Difficulty::Hard)] = StorageService::getHighScore(GameState::MemoryMatching, Difficulty::Hard);

    // Grab Simon High Scores
    highScoreMatrix[static_cast<uint8_t>(GameState::Simon)][static_cast<uint8_t>(Difficulty::Easy)] = StorageService::getHighScore(GameState::Simon, Difficulty::Easy);
    highScoreMatrix[static_cast<uint8_t>(GameState::Simon)][static_cast<uint8_t>(Difficulty::Medium)] = StorageService::getHighScore(GameState::Simon, Difficulty::Medium);
    highScoreMatrix[static_cast<uint8_t>(GameState::Simon)][static_cast<uint8_t>(Difficulty::Hard)] = StorageService::getHighScore(GameState::Simon, Difficulty::Hard);

    // Grab Light Dash High Scores
    highScoreMatrix[static_cast<uint8_t>(GameState::LightDash)][static_cast<uint8_t>(Difficulty::Easy)] = StorageService::getHighScore(GameState::LightDash, Difficulty::Easy);
    highScoreMatrix[static_cast<uint8_t>(GameState::LightDash)][static_cast<uint8_t>(Difficulty::Medium)] = StorageService::getHighScore(GameState::LightDash, Difficulty::Medium);
    highScoreMatrix[static_cast<uint8_t>(GameState::LightDash)][static_cast<uint8_t>(Difficulty::Hard)] = StorageService::getHighScore(GameState::LightDash, Difficulty::Hard);
}

ScoreManager &ScoreManager::Instance()
{
    static ScoreManager instance;
    return instance;
}

uint8_t ScoreManager::getScore()
{
    return score;
}

void ScoreManager::addToScore(int8_t value)
{
    score += value;

    if (score < 0)
    {
        score = 0;
    }

    newHighScore = score > highScoreMatrix[static_cast<uint8_t>(gameState)][static_cast<uint8_t>(difficulty)];
}

void ScoreManager::resetScore(GameState gameState, Difficulty difficulty)
{
    this->gameState = gameState;
    this->difficulty = difficulty;
    score = 0;
    newHighScore = false;
}

void ScoreManager::setHighScore()
{
    if (newHighScore)
    {
        StorageService::saveHighScore(gameState, difficulty, score);
        highScoreMatrix[static_cast<uint8_t>(gameState)][static_cast<uint8_t>(difficulty)] = score;
    }
}

bool ScoreManager::newHighScoreAchieved()
{
    return newHighScore;
}

uint8_t ScoreManager::getHighScore(GameState gameState, Difficulty difficulty)
{
    if (gameState == GameState::None)
        return 0;

    return highScoreMatrix[static_cast<uint8_t>(gameState)][static_cast<uint8_t>(difficulty)];
}
