#include "highscoremanager.h"
#include <avr/boot.h>

const uint8_t HighScoreManager::HIGHSCORE_INITIALIZATION_FLAG = 0x77;

uint8_t HighScoreManager::getHighScore(GameState gameState, Difficulty difficulty)
{
    uint8_t *highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScoreInitFlagAddress = highScoreAddress + 3;

    uint8_t highScoreInitStatus = eeprom_read_byte(highScoreInitFlagAddress);
    return highScoreInitStatus == HIGHSCORE_INITIALIZATION_FLAG ? eeprom_read_byte(highScoreAddress) : 0;
}

void HighScoreManager::saveHighScore(GameState gameState, Difficulty difficulty, uint8_t highScore)
{
    uint8_t *highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScoreInitFlagAddress = highScoreAddress + 3;

    uint8_t highScoreInitStatus = eeprom_read_byte(highScoreInitFlagAddress);

    if (highScoreInitStatus != HIGHSCORE_INITIALIZATION_FLAG)
    {
        eeprom_write_byte(highScoreInitFlagAddress, HIGHSCORE_INITIALIZATION_FLAG);
    }

    eeprom_write_byte(highScoreAddress, highScore);
}

uint8_t *HighScoreManager::getHighScoreAddress(GameState gameState, Difficulty difficulty)
{
    switch (gameState)
    {
    case GameState::MemoryMatching:
        return (uint8_t *)static_cast<int16_t>(difficulty);
    case GameState::Simon:
        return (uint8_t *)static_cast<int16_t>(difficulty) + 6;
    case GameState::LightSpeed:
        return (uint8_t *)static_cast<int16_t>(difficulty) + 12;
    default:
        break;
    }

    return (uint8_t *)40;
}
