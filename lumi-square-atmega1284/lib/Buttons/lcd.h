#ifndef LCD_H
#define LCD_H

#include "stdint.h"

class LCD
{
public:
    LCD(const LCD &) = delete;
    void operator=(const LCD &) = delete;
    static LCD &Instance();
    void displayPower(bool on);
    void clearDisplay();
    void writeString(int8_t row, int8_t column, const char *data);
    void writeNumber(int8_t row, int8_t column, uint8_t value);
    void writeByte(int8_t row, int8_t column, uint8_t data);

private:
    LCD();
    const uint8_t LCD_SLAVE_ADDRESS;
    const uint8_t STARTING_CONTROL_BYTE;
    const uint8_t LAST_CONTROL_BYTE;
    const uint8_t LAST_WRITE_CONTROL_BYTE;
    const uint8_t WRITE_CONTROL_BYTE_CONTINUATION;
    const uint8_t DISPLAY_ON_COMMAND;
    const uint8_t CLEAR_DISPLAY_COMMAND;
    const uint8_t TWO_DISPLAY_LINES_COMMAND;
    const uint8_t ASCII_ZERO;
    const uint8_t ASCII_SPACE;
    const uint8_t heartCharacter[8];
    const uint8_t batteryLowCharacter[8];
    uint8_t displayContentCache[2][16];
    void initializeDisplay();
    void writeDataToRAM(uint8_t dAddress, uint8_t dataByte, uint8_t controlByte);
    void repopulateDisplayFromCache();
    void writeCustomCharacter(const uint8_t character[], uint8_t startingAddress);
    int8_t getRAMAddress(int8_t row, int8_t column);
};

#endif