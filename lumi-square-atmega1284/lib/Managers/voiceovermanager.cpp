#include "voiceovermanager.h"
#include "colors.h"
#include "iostream.h"
#include "leds.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile VoiceOver voiceOverQueue[10] = {VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None, VoiceOver::None};
volatile uint8_t receiveBuffer[10];
volatile uint8_t receiveBufferIndex = 0;
volatile int8_t queueCount = 0;
volatile bool isPlaying = false;

ISR(USART1_RX_vect)
{
    uint8_t data = UDR1;
    receiveBufferIndex++;

    if (receiveBufferIndex == 20 && data == 0xEF)
    {
        receiveBufferIndex = 0;
        isPlaying = false;
    }
}

void VoiceOverManager::Update()
{
    if (queueCount > 0 && !isPlaying)
    {
        isPlaying = true;
        VoiceOver voiceOver = voiceOverQueue[0];
        USART::Instance().usartTransmit(voiceOverData[voiceOver]);

        for (int8_t i = 0; i < queueCount - 1; ++i)
        {
            voiceOverQueue[i] = voiceOverQueue[i + 1];
        }

        --queueCount;
    }
}

bool VoiceOverManager::IsVoiceOverPlaying()
{
    return queueCount > 0 || isPlaying;
}

void VoiceOverManager::PlayVoiceOver(VoiceOver voiceOver)
{
    isPlaying = true;
    USART::Instance().usartTransmit(voiceOverData[voiceOver]);
}

void VoiceOverManager::QueueNumberVoiceOver(uint16_t number)
{
    uint16_t hundreds = (number / 100) * 100;
    uint8_t tens = ((number % 100) / 10) * 10;
    uint8_t ones = number % 10;

    if (hundreds != 0)
    {
        switch (hundreds)
        {
        case 100:
            QueueVoiceOver(VoiceOver::OneHundred);
            break;
        case 200:
            QueueVoiceOver(VoiceOver::TwoHundred);
            break;
        case 300:
            QueueVoiceOver(VoiceOver::ThreeHundred);
            break;
        case 400:
            QueueVoiceOver(VoiceOver::FourHundred);
            break;
        case 500:
            QueueVoiceOver(VoiceOver::FiveHundred);
            break;
        }
    }

    if (tens != 0)
    {
        switch (tens)
        {
        case 10:
            QueueVoiceOver(static_cast<VoiceOver>(tens + ones));
            return;
        case 20:
            QueueVoiceOver(VoiceOver::Twenty);
            break;
        case 30:
            QueueVoiceOver(VoiceOver::Thirty);
            break;
        case 40:
            QueueVoiceOver(VoiceOver::Fourty);
            break;
        case 50:
            QueueVoiceOver(VoiceOver::Fifty);
            break;
        case 60:
            QueueVoiceOver(VoiceOver::Sixty);
            break;
        case 70:
            QueueVoiceOver(VoiceOver::Seventy);
            break;
        case 80:
            QueueVoiceOver(VoiceOver::Eighty);
            break;
        case 90:
            QueueVoiceOver(VoiceOver::Ninety);
            break;
        }
    }

    if (ones != 0)
        QueueVoiceOver(static_cast<VoiceOver>(ones));
}

void VoiceOverManager::QueueVoiceOver(VoiceOver voiceOver)
{
    if (queueCount == 10)
        return;

    voiceOverQueue[queueCount] = voiceOver;
    ++queueCount;
}
