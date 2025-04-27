#include "BootloadUtility.h"
#include <avr/eeprom.h>
#include <string.h>

const uint8_t ack[] = {'\r'};
const uint8_t ctu[] = {'C', 'T', 'U'};
uint8_t dataBuffer[259];

int main(void)
{
    MCUSR = 0;
    wdt_disable();

    uint16_t bufferCounter = 0;
    bool deviceRecognized = false;
    bool writeToFlash = false;
    bool applicationExist = eeprom_read_byte(bootloaderStatusAddress) == uploadCompleteCode;
    bool enterApplication = eeprom_read_byte(applicationEntryStatusAddress) == enterApplicationCode;
    DDRD = 0x00;
    PORTD = 0x00;

    // Return to application section.
    if (applicationExist && enterApplication)
    {
        asm("jmp 0x000");
    }

    // Continue to bootloader section.
    enableUSART();
    startBootloadIndicator();

    if (applicationExist)
    {
        eeprom_update_byte(applicationEntryStatusAddress, enterApplicationCode);
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
        if (dataStruct.data == '\0' && (!writeToFlash || !deviceRecognized))
        {
            bufferCounter = 0;

            if (!memcmp(dataBuffer, "RECOGNIZE", 9))
            {
                usartTransmit(ctu, 3);
                wdt_enable(WDTO_8S);
                deviceRecognized = true;
            }
            else if (!memcmp(dataBuffer, "RTU", 3))
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