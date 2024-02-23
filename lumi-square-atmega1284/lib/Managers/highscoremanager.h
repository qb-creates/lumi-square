#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <stdint.h>
#include "gameproperties.h"

class HighScoreManager
{
public:
    static uint8_t getHighScore(GameState gameState, Difficulty difficulty);
    static void saveHighScore(GameState gameState, Difficulty difficulty, uint8_t highScore);
private:
    HighScoreManager(const HighScoreManager &) = delete;
    void operator=(const HighScoreManager &) = delete;
    static const uint8_t HIGHSCORE_INITIALIZATION_FLAG;
    static uint8_t* getHighScoreAddress(GameState gameState, Difficulty difficulty);
};

#endif