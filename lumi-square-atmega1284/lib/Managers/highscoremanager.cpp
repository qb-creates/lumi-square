#include "highscoremanager.h"
#include <avr/boot.h>

uintptr_t highScoreStatusAddress = 0;

uint8_t HighScoreManager::getHighScore(GameState gameState, Difficulty difficulty)
{
    uint8_t *highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScoreStatusAddress = highScoreAddress + 3;

    uint8_t highscoreSaveStatus = eeprom_read_byte(highScoreStatusAddress);
    return highscoreSaveStatus == 0x00 ? eeprom_read_byte(highScoreAddress) : 0;
}

void HighScoreManager::saveHighScore(GameState gameState, Difficulty difficulty, uint8_t highScore)
{
    uint8_t *highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScoreStatusAddress = highScoreAddress + 3;

    uint8_t highscoreSaveStatus = eeprom_read_byte(highScoreStatusAddress);

    if (highscoreSaveStatus != 0x00)
    {
        eeprom_write_byte(highScoreStatusAddress, 0x00);
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
