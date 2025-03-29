#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "leds.h"
#include "random.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include "voiceovermanager.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <iostream.h>
#include <stdlib.h>
#include <util/delay.h>

int main(void)
{
    Random::configureRNG();
    Input::configureButtonPins();
    Output::configureLeds();
    USART::Instance().initialize();
    FixedUpdateTimer::registerEventHandler(&ShutdownUtility::Instance());
    FixedUpdateTimer::registerEventHandler(&StateManager::Instance());
    FixedUpdateTimer::registerEventHandler(&AudioSource::Instance());
    FixedUpdateTimer::configureFixedUpdate();

    sei();

    while (true)
    {
        if (!FixedUpdateTimer::fixedUpdate)
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        VoiceOverManager::Update();
        FixedUpdateTimer::triggerFixedUpdateEvent();

        // if (FixedUpdateTimer::fixedUpdate)
        // {
        //     LCD::Instance().writeStringToScoreBuffer("did it");
        // }
    }
}