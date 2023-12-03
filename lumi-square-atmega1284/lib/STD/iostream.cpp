/*
 *  SPI.cpp
 *  Author: Quentin Baker
 *
 */
#include "iostream.h"

void std::SPI::configure() const
{
    /* Set SS, SCK, and MOSI as outputs. Set MISO as input */
    DDRB |= _BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPSR = _BV(SPI2X);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(CPOL);
}

void std::SPI::operator<<(volatile uint16_t data) const
{
    SPI::transmit(data >> 8);
    SPI::transmit(data & 0xFF);
    SPI::latch();
}

void std::SPI::transmit(char dData) const
{
    /* Start transmission */
    SPDR = dData;

    /* Wait for transmission complete */
    loop_until_bit_is_set(SPSR, SPIF);
}

void std::SPI::latch() const
{
    PORTB |= _BV(PB4);
    PORTB &= ~_BV(PB4);
}

I2C::I2C() {}

I2C &I2C::Instance()
{
    static I2C instance;
    return instance;
}

void I2C::initialize()
{
    TWSR = 0;
    TWBR = 17;
    TWCR = 0x04;
}

void I2C::start(uint8_t sAddress)
{
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    loop_until_bit_is_set(TWCR, TWINT);

    TWDR = sAddress;
    TWCR = _BV(TWINT) | _BV(TWEN);
    loop_until_bit_is_set(TWCR, TWINT);
}

void I2C::stop()
{
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

void I2C::transmit(char data)
{
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    loop_until_bit_is_set(TWCR, TWINT);
}