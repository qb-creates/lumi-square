#include "serialcommandmanager.h"
#include "shutdownutility.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>

#define USB_CONNECTED (PINC & _BV(PC6))
#define USB_PIN_CHANGE_INT_MASK _BV(PCINT22)
#define USB_PIN_MASK _BV(PC6)

volatile const uint8_t resetCommand[5] = {'R', 'E', 'S', 'E', 'T'};
volatile int16_t uartEnableTimer = 0;
volatile uint8_t resetCommandIndex = 0;
volatile bool uartConfigured = false;
volatile bool uartEnableTimerComplete = false;
volatile bool updating = false;

ISR(USART0_RX_vect)
{
    if (!USB_CONNECTED)
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

    updating = true;

    uint8_t data = UDR0;

    if (data == resetCommand[resetCommandIndex])
    {
        resetCommandIndex++;
    }
    else
    {
        resetCommandIndex = 0;
    }

    if (resetCommandIndex == 5)
    {
        uint8_t *applicationEntryStatusAddress = (uint8_t *)45;
        const uint8_t enterBootloaderCode = 'b';
        eeprom_update_byte((applicationEntryStatusAddress), enterBootloaderCode);
        SerialCommandManager::Instance().usartTransmit(resetCommand[resetCommandIndex - 1]);
        wdt_enable(WDTO_15MS);

        resetCommandIndex = 0;
    }

    _delay_ms(1000);
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
    SerialCommandManager::Instance().usartTransmit(resetCommand[resetCommandIndex - 1]);
}

ISR(PCINT2_vect)
{
    SerialCommandManager::Instance().enableUART(USB_CONNECTED);
    ShutdownUtility::Instance().resetShutdownTimer();
}

SerialCommandManager &SerialCommandManager::Instance(void)
{
    static SerialCommandManager instance;
    return instance;
}

SerialCommandManager::SerialCommandManager() : FixedUpdateEventListener() 
{
    // Enable usb connected pin as input wit no pull up resistor
    DDRC &= ~USB_PIN_MASK;
    PORTC &= ~USB_PIN_MASK;

    // Enable usb connected pin interrupt
    PCMSK2 |= USB_PIN_CHANGE_INT_MASK;

    enableUART(USB_CONNECTED);
}

void SerialCommandManager::onFixedUpdate()
{
    if (!uartEnableTimerComplete && uartEnableTimer > 0)
    {
        uartEnableTimer -= FixedUpdateTimer::DELTA_TIME;

        if (uartEnableTimer <= 0)
        {
            UBRR0L = 119;
            UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
            UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
            uartConfigured = true;
            uartEnableTimerComplete = true;
        }
    }
}

void SerialCommandManager::enableUART(bool enable)
{
    if (enable && !uartEnableTimerComplete)
    {
        uartEnableTimer = 1000;
    }
    else
    {
        UCSR0B = 0;
        UCSR0C = 0;
        uartEnableTimer = 0;
        uartConfigured = false;
        uartEnableTimerComplete = false;
    }
}

void SerialCommandManager::usartTransmit(uint8_t data)
{
    // Wait until the Transmitter is ready
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;
}
