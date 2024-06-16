#include "audio.h"
#include "buttons.h"
#include "leds.h"
#include "random.h"
#include "shutdownutility.h"
#include "statemanager.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <iostream.h>
#include <lcd.h>
#include <stdlib.h>
#include <util/delay.h>
#include "fixedupdate.h"

int main(void)
{
    LCD::Instance().displayPower(true);    
    Random::configureRNG();
    Input::configureButtonPins();
    Output::configureLeds();    
    _delay_ms(100);
    FixedUpdateTimer::registerEventHandler(&ShutdownUtility::Instance());
    FixedUpdateTimer::registerEventHandler(&StateManager::Instance());
    FixedUpdateTimer::registerEventHandler(&AudioSource::Instance());
    FixedUpdateTimer::configureFixedUpdate();
    LCD::Instance().writeByte(0, 14, 0x00);
    LCD::Instance().writeByte(0, 15, 0x01);

    sei();

    while (true)
    {
        if (!FixedUpdateTimer::fixedUpdate)
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        FixedUpdateTimer::triggerFixedUpdateEvent();
        
        if (FixedUpdateTimer::fixedUpdate)
        {
            // LCD::Instance().writeString(0, 0, "it is true");
        }
    }
}