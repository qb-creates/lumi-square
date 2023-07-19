/*
 *  SPI.cpp
 *  Author: Quentin Baker
 *
 */
#include "iostream.h"

void std::SPI::configure() const
{
    /* Set SS, SCK, and MOSI as outputs. Set MISO as input */
    DDRB |= _BV(PB0) | _BV(PB1) | _BV(PB2);

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
    PORTB |= _BV(PB0);
    PORTB &= ~_BV(PB0);
}