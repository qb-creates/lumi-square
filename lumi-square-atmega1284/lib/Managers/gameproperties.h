#ifndef GAMEPROPERTIES_H
#define GAMEPROPERTIES_H

#include <stdint.h>

enum class GameState
{
    None = -1,
    Previous = 0,
    GameOver = 1,
    Menu = 2,
    MemoryMatching = 3,
    LightSpeed = 5,
    Simon = 6
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard
};

class GameProperties
{
public:
    GameProperties(const GameProperties &) = delete;
    void operator=(const GameProperties &) = delete;
    Difficulty gameDifficulty;
    static GameProperties &Instance();
    Difficulty increaseDifficulty();
    void setDifficulty(Difficulty difficulty);

private:
    GameProperties();
    int8_t difficultyCount;
};
#endif