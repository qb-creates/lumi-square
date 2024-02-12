#include "lcd.h"
#include "iostream.h"
#include "string.h"
#include <util/delay.h>

LCD::LCD()
    : LCD_SLAVE_ADDRESS(0x7C),
      STARTING_CONTROL_BYTE(0x80),
      LAST_CONTROL_BYTE(0x00),
      LAST_WRITE_CONTROL_BYTE(0x40),
      WRITE_CONTROL_BYTE_CONTINUATION(0xC0),
      DISPLAY_ON_COMMAND(0x0C),
      CLEAR_DISPLAY_COMMAND(0x01),
      TWO_DISPLAY_LINES_COMMAND(0x28),
      ASCII_ZERO(0x30),
      ASCII_SPACE(0x20),
      heartCharacter{0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00},
      batteryLowCharacter{0x0E, 0x1B, 0x11, 0x11, 0x11, 0x13, 0x1F, 0x1F},
      displayContentCache{{0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
                          {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}} {}

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
 * @brief Turns the LCD display power on or off.
 *
 * This function controls the power state of the LCD display. If the 'on' parameter
 * is set to true, the display is powered on; if it is set to false, the display
 * is powered off.
 *
 * @param on A boolean value indicating whether to power on (true) or off (false) the display.
 *           - true: The display is powered on.
 *           - false: The display is powered off.
 */
void LCD::displayPower(bool on)
{
    if (on)
    {
        DDRC = _BV(PC2);
        PORTC &= ~_BV(PC2);
        _delay_ms(100);

        TWCR &= ~((1 << TWSTO) | (1 << TWEN));
        initializeDisplay();
        repopulateDisplayFromCache();
        return;
    }

    TWCR = 0;
    DDRC = _BV(PC0) | _BV(PC1) | _BV(PC2);
    PORTC = 0xFC;
}

/**
 * @brief Clears all characters from the LCD display.
 *
 * This function sends a command to the LCD display to clear all characters
 * from the display, effectively clearing the entire screen.
 */
void LCD::clearDisplay()
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);
    I2C::Instance().transmit(LAST_CONTROL_BYTE);
    I2C::Instance().transmit(CLEAR_DISPLAY_COMMAND);
    I2C::Instance().stop();
}

/**
 * @brief Writes a string to the specified row and column on the LCD.
 *
 * This function writes the string value to the LCD starting at the specified
 * row and column coordinates. The characters of the string are displayed
 * consecutively on the LCD.
 *
 * @param row The row index on the LCD where the writing should start.
 * @param col The column index on the LCD where the writing should start.
 * @param value The string to be displayed on the LCD.
 */
void LCD::writeString(int8_t row, int8_t column, const char *data)
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);
    size_t len = strlen(data);
    int address = getRAMAddress(row, column);

    for (size_t i = 0; i < len; i++)
    {
        displayContentCache[row][column + i] = data[i];
        uint8_t controlByte = i != (len - 1) ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
        writeDataToRAM(address, data[i], controlByte);
        address++;
    }

    I2C::Instance().stop();
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
 */
void LCD::writeNumber(int8_t row, int8_t column, int32_t value)
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);
    uint8_t digits[3];

    // Extract, hundreds, tens, and ones place
    digits[0] = (value / 100) + ASCII_ZERO;
    digits[1] = ((value / 10) % 10) + ASCII_ZERO;
    digits[2] = (value % 10) + ASCII_ZERO;

    // Replace leading zeros with spaces to ensure leading zeros are not displayed.
    if (digits[0] == ASCII_ZERO)
    {
        digits[0] = ASCII_SPACE;
        digits[1] = digits[1] == ASCII_ZERO ? ASCII_SPACE : digits[1];
    }

    int address = getRAMAddress(row, column);

    for (int i = 0; i < 3; ++i)
    {
        displayContentCache[row][column + i] = digits[i];
        uint8_t controlByte = i != 2 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
        writeDataToRAM(address + i, digits[i], controlByte);
    }

    I2C::Instance().stop();
}

/**
 * @brief Writes a single byte of data to the LCD screen.
 * 
 * This function writes a single byte of data to the LCD at the specified
 * row and column coordinates.
 * 
 * @param row The row index on the LCD screen where the data will be written.
 * @param column The column index on the LCD screen where the data will be written. 
 * @param data The byte of data to be written to the LCD screen.
 */
void LCD::writeByte(int8_t row, int8_t column, uint8_t data)
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);

    int address = getRAMAddress(row, column);
    writeDataToRAM(address, data, LAST_WRITE_CONTROL_BYTE);

    I2C::Instance().stop();
}

void LCD::initializeDisplay()
{
    I2C::Instance().initialize();
    I2C::Instance().start(LCD_SLAVE_ADDRESS);
    I2C::Instance().transmit(STARTING_CONTROL_BYTE);
    I2C::Instance().transmit(DISPLAY_ON_COMMAND);
    I2C::Instance().transmit(LAST_CONTROL_BYTE);
    I2C::Instance().transmit(TWO_DISPLAY_LINES_COMMAND);
    I2C::Instance().stop();

    writeCustomCharacter(heartCharacter, 0x00);
    writeCustomCharacter(batteryLowCharacter, 0x08);
}

void LCD::writeDataToRAM(uint8_t dAddress, uint8_t dataByte, uint8_t controlByte)
{
    I2C::Instance().transmit(STARTING_CONTROL_BYTE);
    I2C::Instance().transmit(dAddress);
    I2C::Instance().transmit(controlByte);
    I2C::Instance().transmit(dataByte);
}

void LCD::repopulateDisplayFromCache()
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);

    for (int row = 0; row < 2; ++row)
    {
        int address = getRAMAddress(row, 0);

        for (int column = 0; column < 16; ++column)
        {
            uint8_t controlByte = column < 16 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE;
            writeDataToRAM((address + column), displayContentCache[row][column], controlByte);
        }
    }

    I2C::Instance().stop();
}

void LCD::writeCustomCharacter(const uint8_t character[], uint8_t startingAddress)
{
    I2C::Instance().start(LCD_SLAVE_ADDRESS);

    for (int i = 0; i < 8; ++i)
    {
        I2C::Instance().transmit(0x80);
        I2C::Instance().transmit(0x40 | startingAddress | i);
        I2C::Instance().transmit(i < 8 ? WRITE_CONTROL_BYTE_CONTINUATION : LAST_WRITE_CONTROL_BYTE);
        I2C::Instance().transmit(character[i]);
    }

    I2C::Instance().stop();
}

int8_t LCD::getRAMAddress(int8_t row, int8_t column)
{
    int address = row == 0 ? 0x80 : 0xC0;
    return address += column;
}
