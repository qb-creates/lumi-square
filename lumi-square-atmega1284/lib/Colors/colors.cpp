#include "colors.h"

const Color Colors::red = Color(255, 0, 0);
const Color Colors::pink = Color(255, 120, 120);
const Color Colors::azure = Color(0, 150, 255);
const Color Colors::purple = Color(255, 0, 255);
const Color Colors::cyan = Color(0, 255, 150);
const Color Colors::aquamarine = Color(0, 255, 20);
const Color Colors::orange = Color(255, 60, 0);
const Color Colors::yellow = Color(255, 255, 0);
const Color Colors::green = Color(0, 255, 0);

/**
 * @brief Default constructor. Constructs a new Color object with r,g,b components defaulted to 0.
 *
 */
Color::Color()
    : r(0), g(0), b(0) {}

/**
 * @brief Constructs a new Color object with given r,g,b components. Enter a value from 0 - 25.
 *
 * @param r Red component value.
 * @param g Green component value.
 * @param b Blue component value.
 */
Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : r(r * .5), g(g), b(b) {}

/**
 * @brief Will compare the r,g,b component of the passed in coller object with this object.
 * Will return true if the r,g,b compoents are equal and false if they are not.
 *
 * @param color The color we are comparing this object to.
 * @return true
 * @return false
 */
bool Color::operator==(Color const &color) const
{
    return (color.r == r && color.g == g && color.b == b);
}

/**
 * @brief 
 *
 * @param color The color we are comparing this object to.
 * @return true
 * @return false
 */
bool Color::operator!=(Color const &color) const
{
    return (color.r != r || color.g != g || color.b != b);
}

/**
 * @brief Copies the r,g,b component values from the passed in color object to this object.
 *
 * @param color The color that will be copied.
 */
void Color::operator=(Color const &color)
{
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
}

Colors::Colors() {}