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
    void initializeDisplay();
    void clearDisplay();
    void writeChars(int8_t row, int8_t column, const char *data);
    void writeInteger(int8_t row, int8_t column, int32_t value);

private:
    LCD();
    const uint8_t sAddress;
};

#endif