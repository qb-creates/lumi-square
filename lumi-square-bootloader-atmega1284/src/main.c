#include <avr/eeprom.h>
#include <string.h>
#include "BootloadUtility.h"

const uint8_t ack[] = {'\r'};

int main(void)
{
    // Clear watchdog reset flag and disable watchdog timer.
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    uint8_t dataBuffer[259];
    uint16_t bufferCounter = 0;
    bool writeToFlash = false;
    bool applicationExist = eeprom_read_byte(bootloaderStatusAddress) == uploadCompleteCode;

    // Return to application section.
    if (applicationExist && (PIND & _BV(PD6)))
    {
        asm("jmp 0x000");
    }

    // Continue to bootloader section.
    enableUSART();
    startBootloadIndicator();

    // Enable CTS output. Set CTS to low to signify that the lumi-square is ready to be updated.
    DDRD |= _BV(PD7);
    PORTD &= ~_BV(PD7);

    if (applicationExist)
    {
        wdt_enable(WDTO_8S);
    }

    while (true)
    {
        // Check for data in the usart receive buffers.
        struct DataElement dataStruct = usartReceive();

        if (!dataStruct.dataReceived)
            continue;

        wdt_reset();
        dataBuffer[bufferCounter] = dataStruct.data;
        ++bufferCounter;

        // Check the data buffer for the "RTU\0" command.
        if (dataStruct.data == '\0' && !writeToFlash)
        {
            bufferCounter = 0;

            if (!memcmp(dataBuffer, "RTU", 4))
            {
                writeToFlash = true;
                startBootloadProcess();
            }

            continue;
        }

        // The microcontroller is writing to flash. Send a byte acknowledgement back to the server.
        if (writeToFlash)
        {
            usartTransmit(ack, 1);
        }

        // Check if the data buffer has been completely filled with page data
        if (bufferCounter == 259)
        {
            bufferCounter = 0;
            writePageDataToFlash(dataBuffer);
        }
    }
}