#include "lcd.h"
#include "iostream.h"
#include "string.h"
#include <util/delay.h>

LCD::LCD()
    : OLED_SLAVE_ADDRESS(0x78),
      COMMAND_CONTROL_BYTE_CONTINUATION(0x80),
      LAST_COMMAND_CONTROL_BYTE(0x00),
      WRITE_CONTROL_BYTE_CONTINUATION(0xC0),
      LAST_WRITE_CONTROL_BYTE(0x40),
      difficultyBuffer(0, 10, 0, 2, 33),
      speakerBuffer(119, 127, 0, 2, 27),
      titleBuffer(15, 22, 0, 3, 32),
      scoreBuffer(15, 22, 4, 7, 32) {}

/**
 * @brief Returns the singleton instance of the LCD class.
 *
 * This static member function provides access to the singleton instance of the LCD class.
 * If an instance of the LCD class does not already exist, it creates a new instance.
 * Subsequent calls to this function return a reference to the existing instance.
 *
 * @return A reference to the singleton instance of the LCD class.
 *
 * @note This function ensures that only one instance of the LCD class exists throughout
 *       the lifetime of the program. It follows the singleton design pattern.
 */
LCD &LCD::Instance()
{
    static LCD instance;
    return instance;
}

/**
 * @brief
 *
 */
void LCD::initializeDisplay(void)
{
    I2C::Instance().initialize();
    I2C::Instance().start(OLED_SLAVE_ADDRESS);

    oledSendCommand(0xAE, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xD5, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x80, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xA8, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x3F, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xD3, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x00, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x40, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x8D, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x14, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x20, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x00, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xA1, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xC8, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xDA, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x12, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x81, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xCF, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xD9, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xF1, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xDB, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0x40, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xA4, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xA6, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0xAF, LAST_COMMAND_CONTROL_BYTE);
    I2C::Instance().stop();

    clearDisplay();
}

/**
 * @brief Clears all characters from the LCD display.
 *
 * This function sends a command to the LCD display to clear all characters
 * from the display, effectively clearing the entire screen.
 */
void LCD::clearDisplay()
{
    I2C::Instance().start(OLED_SLAVE_ADDRESS);
    oledSendCommand(0x21, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(127, COMMAND_CONTROL_BYTE_CONTINUATION);

    oledSendCommand(0x22, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(7, COMMAND_CONTROL_BYTE_CONTINUATION);

    for (int i = 0; i < 1024; i++) // Write Zeros to clear the display
    {
        uint8_t controlByte = i != 1023 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
        oledSendCommand(0, controlByte);
    }

    I2C::Instance().stop();
}

void LCD::writeToDifficultyBuffer(const uint8_t imageData[33])
{
    writeToBuffer(difficultyBuffer, imageData, 0);
}

void LCD::writeToSpeakerBuffer(const uint8_t imageData[27])
{
    writeToBuffer(speakerBuffer, imageData, 0);
}

void LCD::writeStringToTitleBuffer(const char *data)
{
    writeStringToBuffer(titleBuffer, data);
}

void LCD::writeStringToScoreBuffer(const char *data)
{
    writeStringToBuffer(scoreBuffer, data);
}

void LCD::writeStringToBuffer(OLEDBuffer buffer, const char *data)
{
    size_t len = strlen(data);

    for (size_t i = 0; i < len; i++)
    {
        uint8_t charIndex = data[i] - 32;
        writeToBuffer(buffer, letterData[charIndex], (i * 10));
    }
}

void LCD::writeToBuffer(OLEDBuffer buffer, const uint8_t *imageData, uint8_t columnOffset)
{
    I2C::Instance().start(OLED_SLAVE_ADDRESS);
    
    // Set the starting and ending column address
    oledSendCommand(0x21, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(buffer.columnStartAddress + columnOffset, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(buffer.columnEndAddress + columnOffset, COMMAND_CONTROL_BYTE_CONTINUATION);

    // Set the starting and ending page address
    oledSendCommand(0x22, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(buffer.pageStartAddress, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(buffer.pageEndAddress, COMMAND_CONTROL_BYTE_CONTINUATION);

    for (int i = 0; i < buffer.dataCount; i++)
    {
        uint8_t controlByte = i != (buffer.dataCount - 1) ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
        oledSendCommand(imageData[i], controlByte);
    }

    I2C::Instance().stop();
}

void LCD::oledSendCommand(uint8_t command, uint8_t controlByte)
{
    I2C::Instance().transmit(controlByte);
    I2C::Instance().transmit(command);
}

/**
 * @brief Writes an integer value to the specified row and column on the LCD.
 *
 * This function writes the integer value to the LCD starting at the specified
 * row and column coordinates. The integer is formatted as characters and
 * displayed on the LCD.
 *
 * @param row The row index on the LCD where the writing should start.
 * @param col The column index on the LCD where the writing should start.
 * @param value The integer value to be displayed on the LCD.
 * @param leftAlign If true, the numbers will be left-aligned; if false, they will be right-aligned.
 */
// void LCD::writeNumber(int8_t row, int8_t column, uint8_t value, bool leftAlign)
// {
//     I2C::Instance().start(LCD_SLAVE_ADDRESS);
//     uint8_t digits[3];

//     // Extract, hundreds, tens, and ones place
//     digits[0] = (value / 100) + ASCII_ZERO;
//     digits[1] = ((value / 10) % 10) + ASCII_ZERO;
//     digits[2] = (value % 10) + ASCII_ZERO;

//     // Replace leading zeros with spaces to ensure leading zeros are not displayed. Will also left or right align the numbers
//     if (digits[0] == ASCII_ZERO)
//     {
//         if (!leftAlign)
//         {
//             digits[0] = ASCII_SPACE;
//             digits[1] = digits[1] == ASCII_ZERO ? ASCII_SPACE : digits[1];
//         }
//         else
//         {
//             digits[0] = digits[1] != ASCII_ZERO ? digits[1] : digits[2];
//             digits[1] = digits[1] != ASCII_ZERO ? digits[2] : ASCII_SPACE;
//             digits[2] = ASCII_SPACE;
//         }
//     }

//     int address = getRAMAddress(row, column);

//     for (int i = 0; i < 3; ++i)
//     {
//         displayContentCache[row][column + i] = digits[i];
//         uint8_t controlByte = i != 2 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
//         writeDataToRAM(address + i, digits[i], controlByte);
//     }

//     I2C::Instance().stop();
// }
