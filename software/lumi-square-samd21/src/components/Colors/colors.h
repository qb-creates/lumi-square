#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>
#include <stdlib.h>

class Color
{
public:
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b);
    bool operator==(Color const &color) const;
    bool operator!=(Color const &color) const;
    void operator=(Color const &color);
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Colors final
{
public:
    static const Color red;
    static const Color pink;
    static const Color blue;
    static const Color turquoise;
    static const Color purple;
    static const Color orange;
    static const Color yellow;
    static const Color green;

private:
    Colors();
};

#endif