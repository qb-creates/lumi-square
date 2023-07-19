#include "random.h"

/**
 * @brief Configures the Random Number Generator
 * 
 */
void Random::configureRNG()
{
    // Timer 2 is configured in normal mode with no prescaler. TCNT2 will be used for the seed for our Random Generator.
    TCCR2 = _BV(CS20);
}

/**
 * @brief Returns a random integer within the min and the max. Range is inclusive
 * 
 * @param min The minimum value.
 * @param max The maximum value.
 * @return uint8_t 
 */
uint8_t Random::range(uint8_t min, uint8_t max)
{
    srand(TCNT2);
    return min + (rand() % ((max - min) + 1));
}