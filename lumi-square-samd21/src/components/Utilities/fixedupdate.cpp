#include "fixedupdate.h"
#include "../Peripherals/buttons.h"
#include "../Peripherals/leds.h"
#include "samd21j18a.h"

#define FIXED_UPDATE_INTERVAL_MS 33
#define FIXED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - ((F_CPU / 1024UL) * FIXED_UPDATE_INTERVAL_MS) / 1000UL)

#define LED_UPDATE_INTERVAL_US 150ULL
#define LED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - (uint32_t)((((uint64_t)(F_CPU) / 16UL) * LED_UPDATE_INTERVAL_US) / 1000000ULL))

volatile bool FixedUpdateTimer::fixedUpdate = false;
const int8_t FixedUpdateTimer::DELTA_TIME = FIXED_UPDATE_INTERVAL_MS;

extern "C" void TC3_Handler(void)
{
    TC3_REGS->COUNT16.TC_COUNT = FIXED_UPDATE_TIMER_PRELOAD_VALUE;
    TC3_REGS->COUNT16.TC_INTFLAG = 0x01;

    Input::scanButtonMatrix();
    Input::updateSystemButtonStates();
    FixedUpdateTimer::fixedUpdate = true;
}

extern "C" void TC4_Handler(void)
{
    // gives us an overflow timer of .22222ms.
    TC4_REGS->COUNT16.TC_COUNT = LED_UPDATE_TIMER_PRELOAD_VALUE;
    TC4_REGS->COUNT16.TC_INTFLAG = 0x01;
    
    Output::refreshLeds();
}

void FixedUpdateTimer::enableFixedUpdate()
{
    // Timer 3 is configured in normal mode with a prescaler of 1024. Overflow interrupt will be triggered every 33ms.
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC3_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3 | GCLK_CLKCTRL_CLKEN_Msk;

    // 16 bit timer configuration
    TC3_REGS->COUNT16.TC_COUNT = FIXED_UPDATE_TIMER_PRELOAD_VALUE;
    TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk;
    TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_ENABLE_Msk | TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_NFRQ | TC_CTRLA_PRESCALER_DIV1024;
    while ((TC3_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    NVIC_EnableIRQ(TC3_IRQn);

    // Timer 4 is configured in normal mode with a prescaler of 16. Overflow interrupt will be triggered every .22222ms.
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC4_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5 | GCLK_CLKCTRL_CLKEN_Msk;

    // 16 bit timer configuration
    TC4_REGS->COUNT16.TC_COUNT = LED_UPDATE_TIMER_PRELOAD_VALUE;
    TC4_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk;
    TC4_REGS->COUNT16.TC_CTRLA = TC_CTRLA_ENABLE_Msk | TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_NFRQ | TC_CTRLA_PRESCALER_DIV16;
    while ((TC4_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    NVIC_EnableIRQ(TC4_IRQn);
}

void FixedUpdateTimer::disableFixedUpdate()
{
    // Disable Timer0
    // TCCR0B = 0;
    // TIMSK0 = 0;

    // Disable Timer3
    // TCCR3B = 0;
    // TIMSK3 = 0;

    FixedUpdateTimer::fixedUpdate = false;
}
