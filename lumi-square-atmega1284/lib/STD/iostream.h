#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <avr/io.h>
#include <stdint.h>

namespace std
{
class SPI
{
public:
    void configure() const;
    void operator<<(volatile uint16_t data) const;

private:
    void transmit(char data) const;
    void latch() const;
};


static const SPI cout;
} // namespace std
class I2C
{
public:
    I2C(const I2C &) = delete;
    void operator=(const I2C &) = delete;
    static I2C &Instance();
    void initialize();
    void start(uint8_t sAddress);
    void stop();
    void transmit(char data);

private:
    I2C();
};
#endif