#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <stdbool.h>
#include <util/delay.h>

#define POWER_BUTTON_PRESS !(PINB & _BV(PB6))

const int8_t FIXED_UPDATE_MS = 10;
const uint16_t POWER_TOGGLE_TIME = 2000;

bool isOn = false;
uint16_t powerToggleTimer = 0;
int16_t powerOffTimer = 0;

void powerOn(bool powerOn)
{
    PORTA = powerOn ? _BV(PA7) : 0x00;
    isOn = powerOn;
    powerOffTimer = 0;
}

int main(void)
{
    DDRA = 0xFF;
    DDRB = 0x9F;
    PORTA = 0x00;
    PORTB = 0x00;
    power_all_disable();

    _delay_ms(2000);

    while (1)
    {
        // Toggle the power of the lumisquare
        if (POWER_BUTTON_PRESS && powerToggleTimer < POWER_TOGGLE_TIME)
        {
            powerToggleTimer += FIXED_UPDATE_MS;

            if (powerToggleTimer >= POWER_TOGGLE_TIME)
            {
                powerOn(!isOn);
            }
        }
        else if (!POWER_BUTTON_PRESS && powerToggleTimer != 0)
        {
            powerToggleTimer = 0;
        }

        // Power off the lumisquare if no response from the lumisquare microcontroller
        if (isOn)
        {
            powerOffTimer = (PINB & _BV(PB5)) ? powerOffTimer + FIXED_UPDATE_MS : 0;

            if (powerOffTimer > 2000)
                powerOn(false);
        }

        _delay_ms(FIXED_UPDATE_MS);
    }
}