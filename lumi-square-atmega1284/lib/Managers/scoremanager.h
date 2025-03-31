#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "stdio.h"

class ScoreManager
{
public:
    bool newHighScore;

private:
    int16_t score;

public:
    static ScoreManager &Instance();
    int16_t getScore();
    void addToScore(uint8_t value);
    void subtractFromScore(uint8_t value);
    void resetScore();

private:
    ScoreManager();
    ScoreManager(const ScoreManager &) = delete;
    void operator=(const ScoreManager &) = delete;
};

#endif