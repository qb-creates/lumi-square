#include "samd21j18utility.h"
#include "samd21j18a.h"

extern "C" void TC3_Handler(void)
{
    TC3_REGS->COUNT16.TC_COUNT = FIXED_UPDATE_TIMER_PRELOAD_VALUE;
    TC3_REGS->COUNT16.TC_INTFLAG = 0x01;
    DeviceUtility::fixedUpdate = true;
}

SAMD21J18Utility::SAMD21J18Utility() : DeviceUtility(), 
rowOneBaseAddress(0x001), 
rowTwoBaseAddress(0x0010), 
rowThreeBaseAddress(0x0100),
rowFourBaseAddress(0x1000)
{
}

void SAMD21J18Utility::configureFixedUpdateTimer()
{
    // Timer 3 is configured in normal mode with a prescaler of 1024. Overflow interrupt will be triggered every 33ms.
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC3_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3 | GCLK_CLKCTRL_CLKEN_Msk;

    // 16 bit timer configuration
    TC3_REGS->COUNT16.TC_COUNT = FIXED_UPDATE_TIMER_PRELOAD_VALUE;
    TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk;
    TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_ENABLE_Msk | TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_NFRQ | TC_CTRLA_PRESCALER_DIV1024;
    while ((TC3_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk) {}
    NVIC_EnableIRQ(TC3_IRQn);
}

void SAMD21J18Utility::configureButtonPins()
{
    // Enable pullup resistor for next and previous buttons
    // Enable input and pull up resisotrs
    PORT_REGS->GROUP[1].PORT_PINCFG[16] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[17] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[22] |= 0x06;

    // Configure as input and connect to pull up resistor
    PORT_REGS->GROUP[1].PORT_DIR &= ~(PORT_PB16 | PORT_PB17 | PORT_PB22);
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB16 | PORT_PB17 | PORT_PB22;

    // Configure output/inputs for button matrix
    // Enanle input and pull up resistors
    PORT_REGS->GROUP[1].PORT_PINCFG[8] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[9] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[10] |= 0x06;
    PORT_REGS->GROUP[1].PORT_PINCFG[11] |= 0x06;

    // Configure as input and connect to pull up resistor
    PORT_REGS->GROUP[1].PORT_DIR &= ~(PORT_PB08 | PORT_PB09 | PORT_PB10 | PORT_PB11);
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB08 | PORT_PB09 | PORT_PB10 | PORT_PB11;

    // Configure as outputs
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB04 | PORT_PB05 | PORT_PB06 | PORT_PB07;
    PORT_REGS->GROUP[1].PORT_OUT &= ~(PORT_PB04 | PORT_PB05 | PORT_PB06 | PORT_PB07);
}

bool SAMD21J18Utility::getPreviousButtonState()
{
    return !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB16);
}

bool SAMD21J18Utility::getNextButtonState()
{
    return !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB17);
}

bool SAMD21J18Utility::getDifficultyButtonState()
{
    return !(PORT_REGS->GROUP[1].PORT_IN & PORT_PB22);
}

uint32_t SAMD21J18Utility::scanButtonMatrix()
{
    uint32_t buttonData = 0;

    for (uint16_t columnAddress = 0x10; columnAddress < 0x100; columnAddress <<= 1)
    {
        PORT_REGS->GROUP[1].PORT_OUT = (PORT_REGS->GROUP[1].PORT_OUT & 0xFFFFFF0F) | ~columnAddress;

        for (int i = 0; i < 144; i++)
        {
            __NOP();
        }

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB08))
            buttonData |= rowOneBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB09))
            buttonData |= rowTwoBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB10))
            buttonData |= rowThreeBaseAddress * (columnAddress >> 4);

        if (!(PORT_REGS->GROUP[1].PORT_IN & PORT_PB11))
            buttonData |= rowFourBaseAddress * (columnAddress >> 4);
    }

    return buttonData;
}
