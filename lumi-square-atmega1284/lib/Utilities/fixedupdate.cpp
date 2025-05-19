#include "fixedupdate.h"
#include "../Peripherals/buttons.h"
#include "../Peripherals/leds.h"
#include "avr/interrupt.h"

#define FIXED_UPDATE_INTERVAL_MS  33
#define TIMER3_PRELOAD_VALUE (65536UL - ((F_CPU / 256) * FIXED_UPDATE_INTERVAL_MS)  / 1000UL)

volatile bool FixedUpdateTimer::fixedUpdate = false;
const int8_t FixedUpdateTimer::DELTA_TIME = FIXED_UPDATE_INTERVAL_MS;

ISR(TIMER3_OVF_vect)
{
    // Sets bassed off the DELTA_TIME const
    TCNT3 = TIMER3_PRELOAD_VALUE;
    
    Input::scanButtonMatrix();
    Input::updateSystemButtonStates();
    FixedUpdateTimer::fixedUpdate = true;
}

ISR(TIMER0_OVF_vect)
{
    // gives us an overflow timer of .22222ms.
    TCNT0 = 240;

    Output::refreshLeds();
}

void FixedUpdateTimer::enableFixedUpdate()
{
    // Timer 0 is configured in normal mode with a prescaler of 256. Overflow interrupt will be triggered every .22222ms.
    TCCR0B = _BV(CS02);
    TIMSK0 |= _BV(TOIE0);
    TCNT0 = 240;

    // Timer 3 is configured in normal mode with a prescaler of 256. Overflow interrupt will be triggered every 33ms.
    TCCR3B |= _BV(CS32);
    TIMSK3 |= _BV(TOIE3);
    TCNT3 = TIMER3_PRELOAD_VALUE;
}

void FixedUpdateTimer::disableFixedUpdate()
{
    // Disable Timer0
    TCCR0B = 0;
    TIMSK0 = 0;

    // Disable Timer3
    TCCR3B = 0;
    TIMSK3 = 0;

    FixedUpdateTimer::fixedUpdate = false;
}
