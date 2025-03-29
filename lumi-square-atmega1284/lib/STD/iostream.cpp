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
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // pre-scalar 1
    TWBR = ((F_CPU / 400000) - 16) / 2;     // baud rate factor 12
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

USART::USART() {}

USART &USART::Instance()
{
    static USART instance;
    return instance;
}

void USART::initialize(void)
{
    UBRR1L = 119;
    UCSR1B = _BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1);
    UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);

    const uint8_t data[] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x0F, 0xFE, 0xE6, 0xEF};
    usartTransmit(data);
}

void USART::usartTransmit(const uint8_t data[])
{
    for (uint8_t i = 0; i < 10; ++i)
    {
        // Wait until the Transmitter is ready
        loop_until_bit_is_set(UCSR1A, UDRE1);
        UDR1 = data[i];
    }
}
