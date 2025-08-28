#include "random.h"
#include "samd21j18a.h"

/**
 * @brief Configures the Random Number Generator
 * 
 */
void Random::configureRNG()
{
    // Timer 5 is configured in normal mode with no. Overflow interrupt will be triggered every .22222ms.
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC5_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5 | GCLK_CLKCTRL_CLKEN_Msk;

    // 8 bit timer configuration    
    TC5_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk;
    TC5_REGS->COUNT16.TC_CTRLA = TC_CTRLA_ENABLE_Msk | TC_CTRLA_MODE_COUNT8 | TC_CTRLA_WAVEGEN_NFRQ;
    while ((TC5_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
}

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