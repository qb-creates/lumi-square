#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <stdint.h>
#include "gameproperties.h"

class HighScoreManager
{
public:
    HighScoreManager(const HighScoreManager &) = delete;
    void operator=(const HighScoreManager &) = delete;
    static uint8_t getHighScore(GameState gameState, Difficulty difficulty);
    static void saveHighScore(GameState gameState, Difficulty difficulty, uint8_t highScore);
private:
    static uint8_t* getHighScoreAddress(GameState gameState, Difficulty difficulty);
};

#endif