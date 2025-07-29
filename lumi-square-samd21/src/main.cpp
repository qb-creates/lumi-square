#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "leds.h"
#include "random.h"
#include "serialcommandmanager.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include <samd21j18a.h>
#include <system_samd21.h>

#define MAX_FIXED_UPDATE_HANDLERS 10

FixedUpdateEventListener *eventListeners[MAX_FIXED_UPDATE_HANDLERS];
int8_t numHandlers = 0;

volatile uint32_t msTicks = 0;

extern "C" void SysTick_Handler(void)
{
    msTicks++;
}

void delayMS(uint32_t ms)
{
    uint64_t targetTicks = msTicks + ms;
    while (msTicks < targetTicks)
        ;
}

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
    SystemInit();

    delayMS(1000);

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

    while (true)
    {
        if (!FixedUpdateTimer::fixedUpdate)
            continue;

            
        FixedUpdateTimer::fixedUpdate = false;
        triggerFixedUpdateEvent();
    }

    return 1;
}