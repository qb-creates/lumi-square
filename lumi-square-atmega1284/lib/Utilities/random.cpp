#include "random.h"

/**
 * @brief Configures the Random Number Generator
 * 
 */
void Random::configureRNG()
{
    // Timer 2 is configured in normal mode with no prescaler. TCNT2 will be used for the seed for our Random Generator.
    TCCR2B = _BV(CS20);
}

/**
 * @brief 
 * 
 */
void Random::seedRNG()
{
    srand(TCNT2);
}

/**
 * @brief Returns a random integer within the min and the max. Range is inclusive
 * 
 * @param min The minimum value.
 * @param max The maximum value.
 * @return uint8_t 
 */
int16_t Random::range(int16_t min, int16_t max)
{
    return min + (rand() % ((max - min) + 1));
}