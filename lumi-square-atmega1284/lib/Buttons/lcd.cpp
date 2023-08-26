#include "lcd.h"
#include "iostream.h"
#include "string.h"
LCD::LCD()
    : sAddress(0x7C) {}

LCD &LCD::Instance()
{
    static LCD instance;
    return instance;
}

void LCD::initializeDisplay()
{
    I2C::Instance().start(sAddress);
    I2C::Instance().transmit(0x80);
    I2C::Instance().transmit(0x0C);
    I2C::Instance().transmit(0x00);
    I2C::Instance().transmit(0x28);
    I2C::Instance().stop();
}

void LCD::clearDisplay()
{
    I2C::Instance().start(sAddress);
    I2C::Instance().transmit(0x00);
    I2C::Instance().transmit(0x01);
    I2C::Instance().stop();
}

void LCD::writeChars(int8_t row, int8_t column, const char *data)
{
    size_t len = strlen(data);
    I2C::Instance().start(sAddress);
    int address = row == 0 ? 0x80 : 0xC0;
    address += column;

    for (size_t i = 0; i < len; i++)
    {
        // Write data
        I2C::Instance().transmit(0x80);
        I2C::Instance().transmit(address);
        I2C::Instance().transmit(i != (len - 1) ? 0xC0 : 0x40);
        I2C::Instance().transmit(data[i]);

        address++;
        if (address == 0x90)
        {
            address = 0xC0;
        }
    }

    I2C::Instance().stop();
}

void LCD::writeInteger(int8_t row, int8_t column, int32_t value)
{
    I2C::Instance().start(sAddress);
    int address = row == 0 ? 0x80 : 0xC0;
    address += column;
    
    char temp[5]{0, 0, 0, 0, 0};
    int counter = 0;
    do
    {
        int mod = value % 10;
        value /= 10;
        temp[counter] = mod;

        counter++;

    } while (value != 0);

    for (int i = counter - 1; i >= 0; --i)
    {
        I2C::Instance().transmit(0x80);
        I2C::Instance().transmit(address);
        I2C::Instance().transmit(0xC0);
        I2C::Instance().transmit(0x30 + temp[i]);

        address++;
        if (address == 0x90)
        {
            address = 0xC0;
        }
    }
    I2C::Instance().stop();
}