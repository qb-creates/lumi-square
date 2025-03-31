#include "fixedupdate.h"
#include "../Peripherals/buttons.h"
#include "../Peripherals/leds.h"
#include "avr/interrupt.h"

volatile bool FixedUpdateTimer::fixedUpdate = false;
const int8_t FixedUpdateTimer::MAX_HANDLERS = 10;
const int8_t FixedUpdateTimer::DELTA_TIME = 33;
int8_t FixedUpdateTimer::numHandlers = 0;
FixedUpdateEventListener *FixedUpdateTimer::eventListeners[10];

ISR(TIMER3_OVF_vect)
{
    // gives us an overflow timer of 33ms.
    TCNT3 = 63159;
    
    Input::scanButtonMatrix();
    Input::updateSystemButtonStates();
    FixedUpdateTimer::fixedUpdate = true;
}

ISR(TIMER0_OVF_vect)
{
    // gives us an overflow timer of .75ms.
    TCNT0 = 201;

    Output::refreshLeds();
}

void FixedUpdateTimer::configureFixedUpdate()
{
    // Timer 0 is configured in normal mode with a prescaler of 256. Overflow interrupt will be triggered every .75ms.
    TCCR0B = _BV(CS02);
    TIMSK0 |= _BV(TOIE0);
    TCNT0 = 201;

    // Timer 3 is configured in normal mode with a prescaler of 256. Overflow interrupt will be triggered every 33ms.
    TCCR3B |= _BV(CS32);
    TIMSK3 |= _BV(TOIE3);
    TCNT3 = 63159;
}

void FixedUpdateTimer::registerEventHandler(FixedUpdateEventListener *listener)
{
    if (numHandlers < 10)
    {
        eventListeners[numHandlers++] = listener;
    }
}

void FixedUpdateTimer::triggerFixedUpdateEvent()
{
    for (int i = 0; i < numHandlers; ++i)
    {
        if (eventListeners[i] != nullptr)
        {
            eventListeners[i]->onFixedUpdate();
        }
    }
}
