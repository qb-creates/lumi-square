#include "random.h"
#include "audio.h"
#include "leds.h"
#include "buttons.h"
#include "statemanager.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

volatile bool fixedUpdate = false;
volatile uint8_t fixedUpdateCounter = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 141;
    Input::scanButtonMatrix();
    Output::refreshLeds();

    if (++fixedUpdateCounter == 16)
    {
        fixedUpdateCounter = 0;
        fixedUpdate = true;
    }
}

int main(void)
{
    // Timer 0 is configured in normal mode with a prescaler of 64
    TCCR0 = _BV(CS02);

    TIMSK |= _BV(TOIE0);

    Random::configureRNG();
    AudioSource::configureAudioSource();
    Input::configureButtonMatrix();
    Output::configureLeds();
    sei();

    while (true)
    {
        if (fixedUpdate)
        {
            AudioSource::updateAudioSource();
            StateManager::Instance().buttonPressed();
            StateManager::Instance().update();
            fixedUpdate = false;
        }
    }
}