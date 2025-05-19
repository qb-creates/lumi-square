#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "leds.h"
#include "random.h"
#include "serialcommandmanager.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#define MAX_FIXED_UPDATE_HANDLERS 10

FixedUpdateEventListener *eventListeners[MAX_FIXED_UPDATE_HANDLERS];
int8_t numHandlers = 0;

/**
 * @brief Will register a FixedUpdateEvent
 *
 * @param listener
 */
void registerFixedUpdateEventHandler(FixedUpdateEventListener *listener)
{
    if (numHandlers < MAX_FIXED_UPDATE_HANDLERS)
    {
        eventListeners[numHandlers++] = listener;
    }
}

/**
 * @brief
 *
 */
void triggerFixedUpdateEvent()
{
    for (int i = 0; i < numHandlers; ++i)
    {
        if (eventListeners[i] != nullptr)
        {
            eventListeners[i]->onFixedUpdate();
        }
    }
}

int main(void)
{
    _delay_ms(1000);

    // Configure
    Random::configureRNG();
    Input::configureButtonPins();
    Output::configureLeds();

    // Register Events
    registerFixedUpdateEventHandler(&StateManager::Instance());
    registerFixedUpdateEventHandler(&AudioSource::Instance());
    registerFixedUpdateEventHandler(&ShutdownUtility::Instance());
    registerFixedUpdateEventHandler(&SerialCommandManager::Instance());
    FixedUpdateTimer::enableFixedUpdate();

    sei();

    while (true)
    {
        if (!FixedUpdateTimer::fixedUpdate)
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        triggerFixedUpdateEvent();
    }
}