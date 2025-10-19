#include "audio.h"
#include "buttons.h"
#include "deviceutility.h"
#include "ledmatrix.h"
#include "random.h"
#include "serialcommandmanager.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include <samd21j18a.h>
#include <system_samd21.h>

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

int main(void)
{
    SystemInit();

    delayMS(1000);

    // Configure
    DeviceUtility::Instance().configure();

    while (true)
    {
        LEDMatrix::refreshLeds();

        if (!DeviceUtility::fixedUpdate)
            continue;

        DeviceUtility::fixedUpdate = false;
        Input::pollButtons();

        StateManager::Instance().onFixedUpdate();
        AudioSource::Instance().onFixedUpdate();
        ShutdownUtility::Instance().onFixedUpdate();
        SerialCommandManager::Instance().onFixedUpdate();
    }

    return 1;
}