#include "fixedupdate.h"
#include "../Peripherals/buttons.h"
#include "../Peripherals/leds.h"
#include "avr/interrupt.h"

volatile bool FixedUpdateTimer::fixedUpdate = false;
volatile uint8_t FixedUpdateTimer::fixedUpdateCounter = 0;
const int FixedUpdateTimer::MAX_HANDLERS = 10;
int FixedUpdateTimer::numHandlers = 0;
FixedUpdateEventListener *FixedUpdateTimer::eventListeners[10];

ISR(TIMER3_OVF_vect)
{
    // gives us an overflow timer of 16ms.
    TCNT3 = 64383;
    FixedUpdateTimer::fixedUpdate = true;
}

ISR(TIMER0_OVF_vect)
{
    // gives us an overflow timer of .25ms.
    TCNT0 = 237;

    Input::scanButtonMatrix();
    Input::updateSystemButtonStates();
    Output::refreshLeds();
}

void FixedUpdateTimer::configureFixedUpdate()
{
    // Timer 0 is configured in normal mode with a prescaler of 256. Needs to be set up for 1ms updates
    TCCR0B = _BV(CS02);

    TIMSK0 |= _BV(TOIE0);

    TCNT0 = 237;

    // Timer 
    TCCR3B |= _BV(CS32);
    TIMSK3 |= _BV(TOIE3);
    TCNT3 = 64383;
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
