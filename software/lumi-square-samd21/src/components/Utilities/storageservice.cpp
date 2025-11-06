#include "storageservice.h"
// #include <samd21j18a.h>
#include <string.h>

#define NVM_ROW_SIZE_BYTES 256
#define NVM_PAGE_SIZE_BYTES 64

// __attribute__((__section__(".eeprom")))
// const uint32_t eepromData[256] = {0};


uint8_t read_flash_byte(uint32_t offset)
{
    // return eepromData[offset];
    return 0;
}

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
uint8_t StorageService::getHighScore(GameState gameState, Difficulty difficulty)
{
    uint32_t highScoreAddress = getHighScoreAddress(gameState, difficulty);
    uint8_t *highScorePtr = (uint8_t *)highScoreAddress;
    return *highScorePtr == 255 ? 0 : *highScorePtr;
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
void StorageService::saveHighScore(GameState gameState, Difficulty difficulty, uint8_t highScore)
{
    // uint32_t highScoreAddress = getHighScoreAddress(gameState, difficulty);

    // // Align to row (256-byte boundary)
    // uintptr_t rowStart = highScoreAddress & ~0xFF;
    // uint8_t buffer[NVM_ROW_SIZE_BYTES];

    // // Step 1: Copy current flash row to RAM
    // memcpy(buffer, (void *)rowStart, NVM_ROW_SIZE_BYTES);

    // // Step 2: Modify the byte you care about
    // buffer[highScoreAddress - rowStart] = highScore;

    // // Step 3: Erase the row
    // NVMCTRL_REGS->NVMCTRL_ADDR = rowStart / 2; // Flash address in half-words
    // NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_ER;
    // while (!(NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk)) {}

    // // Step 4: Write back all 4 pages (256 bytes)
    // for (int page = 0; page < 4; page++)
    // {
    //     uint32_t *dst = (uint32_t *)(rowStart  + (page * NVM_PAGE_SIZE_BYTES));
    //     uint32_t *src = (uint32_t *)&buffer[page * NVM_PAGE_SIZE_BYTES];

    //     // Write page data to page buffer
    //     for (int i = 0; i < NVM_PAGE_SIZE_BYTES / 4; i++)
    //     {
    //         dst[i] = src[i];
    //     }

    //     // Execute write page
    //     NVMCTRL_REGS->NVMCTRL_ADDR = ((uint32_t)dst) / 2;
    //     NVMCTRL_REGS->NVMCTRL_CTRLA = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
    //     while (!(NVMCTRL_REGS->NVMCTRL_INTFLAG & NVMCTRL_INTFLAG_READY_Msk)) {}
    // }
}

uint32_t StorageService::getHighScoreAddress(GameState gameState, Difficulty difficulty)
{
    // uintptr_t offset = 0;

    // switch (gameState)
    // {
    // case GameState::MemoryMatching:
    //     offset = (uint32_t)static_cast<int16_t>(difficulty);
    //     break;
    // case GameState::Simon:
    //     offset = (uint32_t)(static_cast<int16_t>(difficulty) + 6);
    //     break;
    // case GameState::LightDash:
    //     offset = (uint32_t)(static_cast<int16_t>(difficulty) + 12);
    //     break;
    // default:
    //     return (uintptr_t)eepromData;
    // }

    // return (uintptr_t)(eepromData + offset);
}
