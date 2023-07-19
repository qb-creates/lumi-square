#ifndef IOSTREAM_H
#define IOSTREAM_H
#include <stdint.h>
#include <avr/io.h>

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
}
#endif