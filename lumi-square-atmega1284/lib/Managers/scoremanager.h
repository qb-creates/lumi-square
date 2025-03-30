#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include "stdio.h"

class ScoreManager
{
public:
    static bool newHighScore;

private:
    static int16_t score;

public:
    static int16_t getScore();
    static void addToScore(uint8_t value);
    static void subtractFromScore(uint8_t value);
    static void resetScore();

private:
    ScoreManager();
    ScoreManager(const ScoreManager &) = delete;
    void operator=(const ScoreManager &) = delete;
};

#endif