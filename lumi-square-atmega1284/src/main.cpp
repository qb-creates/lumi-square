#include "audio.h"
#include "buttons.h"
#include "fixedupdate.h"
#include "leds.h"
#include "random.h"
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

volatile const uint8_t resetCommand[5] = {'R', 'E', 'S', 'E', 'T'};
volatile int16_t uartTimer = 0;
volatile uint8_t resetCommandIndex = 0;
bool receivingData = false;
volatile bool uartConfigured = false;
volatile bool uartConnectedTimerComplete = false;
volatile bool updating = false;

void usartTransmit(uint8_t data)
{
    // Wait until the Transmitter is ready
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}

ISR(USART0_RX_vect)
{
    if (!(PINC & _BV(PC6)))
        return;

    UCSR0B = _BV(RXEN0) | _BV(TXEN0);

    if (!updating)
    {
        DDRD |= _BV(PD5);
        PORTD &= ~_BV(PD5);
        wdt_enable(WDTO_4S);
        FixedUpdateTimer::disableFixedUpdate();
    }

    wdt_reset();
    uartConfigured = false;
    updating = true;

    uint8_t data = UDR0;

    if (data == resetCommand[resetCommandIndex]) {
        resetCommandIndex++;
    } else {
        resetCommandIndex = 0;
    }

    if (resetCommandIndex == 5)
    {
        uint8_t *applicationEntryStatusAddress = (uint8_t *)45;
        const uint8_t enterBootloaderCode = 'b';
        eeprom_update_byte((applicationEntryStatusAddress), enterBootloaderCode);
        usartTransmit(resetCommand[resetCommandIndex - 1]);
        wdt_enable(WDTO_15MS);

        resetCommandIndex = 0;
    }
}

void enableUART(void)
{
    if (!(PINC & _BV(PC6)))
    {
        UCSR0B = 0;
        UCSR0C = 0;
        uartTimer = 0;
        uartConfigured = false;
        uartConnectedTimerComplete = false;
    }
    else if ((PINC & _BV(PC6)))
    {
        uartTimer = 1000;
    }
}

ISR(PCINT2_vect)
{
    enableUART();
    ShutdownUtility::Instance().resetShutdownTimer();
}


int main(void)
{
    // Detect if usb connected
    DDRC &= ~_BV(PC6);
    PORTC &= ~_BV(PC6);
    enableUART();
    _delay_ms(1000);


    Random::configureRNG();
    Input::configureButtonPins();
    Output::configureLeds();
    FixedUpdateTimer::registerEventHandler(&StateManager::Instance());
    FixedUpdateTimer::registerEventHandler(&AudioSource::Instance());
    FixedUpdateTimer::registerEventHandler(&ShutdownUtility::Instance());
    FixedUpdateTimer::configureFixedUpdate();
    sei();

    while (true)
    {
        if (uartConnectedTimerComplete && !uartConfigured)
        {
            _delay_ms(1000);
            uartConfigured = true;
            usartTransmit(resetCommand[resetCommandIndex - 1]);
            UCSR0B |= _BV(RXCIE0);
        }

        if (!FixedUpdateTimer::fixedUpdate)
            continue;

        FixedUpdateTimer::fixedUpdate = false;
        FixedUpdateTimer::triggerFixedUpdateEvent();

        if (!uartConnectedTimerComplete && uartTimer > 0)
        {
            uartTimer -= FixedUpdateTimer::DELTA_TIME;

            if (uartTimer <= 0)
            {
                UBRR0L = 119;
                UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
                UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
                uartConfigured = true;
                uartConnectedTimerComplete = true;
            }                
        }
    }
}