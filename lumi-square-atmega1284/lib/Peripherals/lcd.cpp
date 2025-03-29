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
      difficultyBuffer(117, 127, 0, 2, 33),
      speakerBuffer(119, 127, 0, 2, 27),
      topBuffer(0, 7, 0, 3, 32),
      bottomBuffer(0, 7, 4, 7, 32),
      bottomLeftBufferLength(0),
      bottomRightBufferLength(0) {}

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

void LCD::clearTopBuffer()
{
    writeToTopBuffer("             ");
}

void LCD::clearBottomBuffer()
{
    I2C::Instance().start(OLED_SLAVE_ADDRESS);
    oledSendCommand(0x21, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(0, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(127, COMMAND_CONTROL_BYTE_CONTINUATION);

    oledSendCommand(0x22, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(4, COMMAND_CONTROL_BYTE_CONTINUATION);
    oledSendCommand(7, COMMAND_CONTROL_BYTE_CONTINUATION);

    for (int i = 0; i < 512; i++) // Write Zeros to clear the display
    {
        uint8_t controlByte = i != 512 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
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

void LCD::writeToTopBuffer(const char *data)
{
    writeStringToBuffer(topBuffer, data);
}

void LCD::writeToBottomBuffer(const char *data)
{
    writeStringToBuffer(bottomBuffer, data);
}

void LCD::writeToBottomLeftBuffer(uint8_t value)
{
    writeNumberToBuffer(bottomBuffer, value, &bottomLeftBufferLength);
}

void LCD::writeToBottomRightBuffer(uint8_t value)
{
    writeNumberToBuffer(bottomBuffer, value, &bottomRightBufferLength);
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

void LCD::writeNumberToBuffer(OLEDBuffer buffer, uint8_t value, uint8_t *test)
{
    char digits[4];
    sprintf(digits, "%u", value);
    size_t len = strlen(digits);
    uint8_t offset = test == &bottomLeftBufferLength ? 10 : 80;

    if (*test != len)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            writeToBuffer(buffer, letterData[0], (i * 8) + offset);
        }

        *test = len;
    }

    offset += (3 - len) * 5;

    for (size_t i = 0; i < len; i++)
    {
        uint8_t charIndex = digits[i] - 32;
        writeToBuffer(buffer, letterData[charIndex], (i * 10) + offset);
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
