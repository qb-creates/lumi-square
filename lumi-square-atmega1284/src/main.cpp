#include "audio.h"
#include "buttons.h"
#include "leds.h"
#include "random.h"
#include "sleepmanager.h"
#include "statemanager.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <iostream.h>
#include <lcd.h>
#include <stdlib.h>
#include <util/delay.h>

volatile uint8_t fixedUpdateCounter = 0;
volatile uint16_t sleepTimer = 0;
volatile bool fixedUpdate = false;
volatile bool enterSleepMode = false;

bool enableAudioMute = false;

ISR(TIMER0_OVF_vect)
{
    // gives us an overflow timer of .25ms.
    TCNT0 = 237;

    bool buttonPressed = Input::scanButtonMatrix();

    if (buttonPressed)
        SleepManager::resetSleepTimer();

    Input::updateSystemButtonStates();
    Output::refreshLeds();

    if (++fixedUpdateCounter == 64)
    {
        fixedUpdateCounter = 0;
        fixedUpdate = true;
    }
}

int16_t sleepButtonTimer = 0;

int main(void)
{
    // Turn on lcd screen
    LCD::Instance().displayPower(true);

    // Timer 0 is configured in normal mode with a prescaler of 256. Needs to be set up for 1ms updates
    TCCR0B = _BV(CS02);

    TIMSK0 |= _BV(TOIE0);

    Random::configureRNG();
    AudioSource::configureAudioSource();
    Input::configureButtonPins();
    Output::configureLeds();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sei();

    while (true)
    {
        if (!fixedUpdate)
            continue;
    
        if (Input::getSleepButton() && sleepButtonTimer < 1200)
        {
            sleepButtonTimer += 16;

            if (sleepButtonTimer >= 1200)
            {
                AudioSource::playMusicNote(MusicNote::A5, 100);
            }
        }

        if (Input::getSleepButtonUp())
        {
            if (sleepButtonTimer >= 1200)
            {
                SleepManager::enterSleepMode();
            }

            sleepButtonTimer = 0;
        }

        if (Input::getMuteButtonDown())
        {
            enableAudioMute = !enableAudioMute;
            AudioSource::muteAudioSource(enableAudioMute);
        }

        AudioSource::playNextAudioClipNote();
        AudioSource::updateMusicNoteTimer();
        StateManager::Instance().buttonPressed();
        StateManager::Instance().update();
        Input::clearSystemButtonStates();
        SleepManager::updateSleepTimer(16);
        fixedUpdate = false;
    }
}