#include "audio.h"
#include "buttons.h"
#include "leds.h"
#include "random.h"
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

ISR(TIMER0_OVF_vect)
{
    TCNT0 = 183; // gives us an overflow timer of 1ms.
    bool buttonPressed = Input::scanButtonMatrix();

    if (buttonPressed)
        sleepTimer = 0;

    Output::refreshLeds();

    if (++fixedUpdateCounter == 16)
    {
        fixedUpdateCounter = 0;
        fixedUpdate = true;
    }
}

ISR(INT0_vect)
{
    if (!enterSleepMode)
        return;

    EIMSK = 0;
    EICRA = 0;
    enterSleepMode = false;
    LCD::Instance().displayPower(true);
}

int main(void)
{
    // Enable pullup resistor for sleep button
    DDRD &= ~_BV(PD2);
    PORTD |= _BV(PD2);

    // Turn on lcd screen
    LCD::Instance().displayPower(true);

    // Timer 0 is configured in normal mode with a prescaler of 256. Needs to be set up for 1ms updates
    TCCR0B = _BV(CS02);

    TIMSK0 |= _BV(TOIE0);

    Random::configureRNG();
    AudioSource::configureAudioSource();
    Input::configureButtonMatrix();
    Output::configureLeds();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sei();

    while (true)
    {
        if (fixedUpdate)
        {
            if (sleepTimer >= 20000)
            {
                LCD::Instance().displayPower(false);
                Random::seedRNG();
                sleepTimer = 0;
                enterSleepMode = true;
                EIMSK = _BV(INT0);
                EICRA = 0;
                sleep_mode();
            }

            if (!enterSleepMode)
            {
                AudioSource::playNextAudioClipNote();
                AudioSource::updateMusicNoteTimer();
                StateManager::Instance().buttonPressed();
                StateManager::Instance().update();
                fixedUpdate = false;

                sleepTimer += 16;
            }
        }
    }
}

// ISR(INT4_vect)
// {
//     EIMSK = 0;
//     EICRB = 0;
//     PORTE &= ~_BV(PE0);
//     _delay_ms(100);
//     DDRD &= 0xFC;
//     TWCR &= ~((1 << TWSTO) | (1 << TWEN));
//     I2C::Instance().initialize();
//     LCD::Instance().initializeDisplay();
//     LCD::Instance().writeChars(0, 0, "Test this");
//     goToSleep = false;
// }
// int main(void)
// {
//     // Turn on lcd screen
//     DDRE |= _BV(PE0);
//     PORTE = _BV(PE4);
//     _delay_ms(100);

//     // Timer 0 is configured in normal mode with a prescaler of 64
//     TCCR0 = _BV(CS02);

//     TIMSK |= _BV(TOIE0);

//     I2C::Instance().initialize();
//     LCD::Instance().initializeDisplay();
//     Random::configureRNG();
//     AudioSource::configureAudioSource();
//     Input::configureButtonMatrix();
//     Output::configureLeds();

//     int16_t sleepTimer = 5000;

//     set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//     sei();

//     while (true)
//     {
//         if (fixedUpdate)
//         {
//             if (!(PINE & _BV(PE4)) && sleepTimer > 0)
//             {
//                 sleepTimer -= 16;

//                 if (sleepTimer <= 0)
//                     goToSleep = true;
//             }
//             else if ((PINE & _BV(PE4)) && sleepTimer <= 0)
//             {
//                 _delay_ms(100);
//                 sleepTimer = 5000;
//                 EIMSK = _BV(INT4);
//                 sleep_mode();
//             }

//             if (newSleepTiemr >= 5000)
//             {
//                 PORTE |= _BV(PE0);
//                 TWCR = 0;
//                 DDRD |= 0x03;
//                 PORTD &= 0xFC;
//                 newSleepTiemr = 0;
//                 EIMSK = _BV(INT4);
//                 sleep_mode();
//             }

// if (!(PINE & _BV(PE4)))
// {

//     AudioSource::playNote(MusicNote::A5, 100);
// }
// if (TIFR & _BV(TOV0))
// {
//     TCNT0 = 141;
//     TIFR |= _BV(TOV0);
//     Input::scanButtonMatrix();
//     Output::refreshLeds();

//     if (++fixedUpdateCounter == 16)
//     {
//         fixedUpdateCounter = 0;
//         fixedUpdate = true;
//     }
// }