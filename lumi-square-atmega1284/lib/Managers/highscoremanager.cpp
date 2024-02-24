#include "highscoremanager.h"
#include <avr/boot.h>

const uint8_t HighScoreManager::HIGHSCORE_INITIALIZATION_FLAG = 0x77;

/**
 * @brief Retrieves the current high score for the specified game.
 *
 * Retrieves the high score for the specified game state and difficulty level.
 * Checks if the high score is initialized before returning its value.
 *
 * @param gameState The name of the game for which the high score is retrieved.
 * @param difficulty The difficulty level of the game.
 * @return The current high score for the specified game state and difficulty, or 0 if a high score hasn't been set.
 */
uint8_t HighScoreManager::getHighScore(GameState gameState, Difficulty difficulty)
{
    uint8_t *highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScoreInitFlagAddress = highScoreAddress + 3;

    uint8_t highScoreInitStatus = eeprom_read_byte(highScoreInitFlagAddress);
    return highScoreInitStatus == HIGHSCORE_INITIALIZATION_FLAG ? eeprom_read_byte(highScoreAddress) : 0;
}

/**
 * @brief Sets the high score for the specified game.
 *
 * Sets the high score for the specified game and difficulty level to the given value.
 *
 * @param gameState The name of the game for which the high score is set.
 * @param difficulty The difficulty level of the game.
 * @param highScore The high score value to be set.
 */
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
    case GameState::LightDash:
        return (uint8_t *)static_cast<int16_t>(difficulty) + 12;
    default:
        break;
    }

    return (uint8_t *)40;
}
