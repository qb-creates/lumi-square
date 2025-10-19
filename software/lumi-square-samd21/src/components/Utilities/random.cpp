#include "random.h"
#include "samd21j18a.h"

/**
 * @brief 
 * 
 */
void Random::seedRNG()
{
    srand(TC5_REGS->COUNT16.TC_COUNT);
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