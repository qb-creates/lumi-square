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

int main(void)
{
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
        if (!FixedUpdateTimer::fixedUpdate)
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        FixedUpdateTimer::triggerFixedUpdateEvent();

        // if (FixedUpdateTimer::fixedUpdate)
        // {
        //     LCD::Instance().writeStringToScoreBuffer("did it");
        // }
    }
}