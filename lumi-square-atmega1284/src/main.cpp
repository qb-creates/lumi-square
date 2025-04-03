#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "leds.h"
#include "random.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/wdt.h>

int main(void)
{
    _delay_ms(1000);
    DDRC &= ~_BV(PC6);
    PORTC &= ~_BV(PC6);

    wdt_enable(WDTO_4S);
    Random::configureRNG();
    Input::configureButtonPins();
    Output::configureLeds();
    FixedUpdateTimer::registerEventHandler(&AudioSource::Instance());
    FixedUpdateTimer::registerEventHandler(&StateManager::Instance());
    FixedUpdateTimer::registerEventHandler(&ShutdownUtility::Instance());
    FixedUpdateTimer::configureFixedUpdate();

    sei();

    while (true)
    {
        if (!FixedUpdateTimer::fixedUpdate || (PINC & _BV(PC6)))
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        FixedUpdateTimer::triggerFixedUpdateEvent();        
        wdt_enable(WDTO_2S);
        // if (FixedUpdateTimer::fixedUpdate)
        // {
        //     LCD::Instance().writeStringToScoreBuffer("did it");
        // }
    }
}