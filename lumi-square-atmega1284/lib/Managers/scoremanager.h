#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "gameproperties.h"
#include "stdio.h"
#include "storageservice.h"

class ScoreManager
{
private:
    bool newHighScore;
    int16_t score;
    uint8_t highScoreMatrix[6][3];
    GameState gameState;
    Difficulty difficulty;

public:
    static ScoreManager &Instance();
    uint8_t getScore();
    void addToScore(int8_t value);
    void resetScore(GameState gameState, Difficulty difficulty); // Rename this
    void setHighScore(); // rename this
    bool newHighScoreAchieved();
    uint8_t getHighScore(GameState gameState, Difficulty difficulty); // possibly remove

private:
    ScoreManager();
    ScoreManager(const ScoreManager &) = delete;
    void operator=(const ScoreManager &) = delete;
};

#endif