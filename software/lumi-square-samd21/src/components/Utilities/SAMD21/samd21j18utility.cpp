#include "samd21j18utility.h"
#include "led.h"
#include "samd21j18a.h"
#include <string.h>

#define DMA_CH_UART_TX 0;

static const uint8_t DF_PLAYER_COMMANDS[46][10] = {
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x01, 0xFE, 0xEA, 0xEF}, // 0
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x02, 0xFE, 0xE9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x03, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x04, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x05, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x06, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x07, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x08, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x09, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x0A, 0xFE, 0xE1, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x01, 0xFE, 0xE9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x02, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x03, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x04, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x05, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x06, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x07, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x08, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x09, 0xFE, 0xE1, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0A, 0xFE, 0xE0, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0B, 0xFE, 0xDF, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0C, 0xFE, 0xDE, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0D, 0xFE, 0xDD, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0E, 0xFE, 0xDC, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x0F, 0xFE, 0xDB, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x10, 0xFE, 0xDA, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x11, 0xFE, 0xD9, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x02, 0x12, 0xFE, 0xD8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x01, 0xFE, 0xE8, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x02, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x03, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x04, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x03, 0x05, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x01, 0xFE, 0xE7, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x02, 0xFE, 0xE6, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x03, 0xFE, 0xE5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x04, 0xFE, 0xE4, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x05, 0xFE, 0xE3, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x06, 0xFE, 0xE2, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x07, 0xFE, 0xE1, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x08, 0xFE, 0xE0, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x09, 0xFE, 0xDF, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x0A, 0xFE, 0xDE, 0xEF},
    {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x04, 0x0B, 0xFE, 0xDD, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xFE, 0xF5, 0xEF},
    {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x0F, 0xFE, 0xE6, 0xEF}};
static const uint8_t MUTE_COMMAND_RESPONSE[20] = {0x7E, 0xFF, 0x06, 0x3D, 0x00, 0x00, 0x2B, 0xFE, 0x93, 0xEF, 0x7E, 0xFF, 0x06, 0x3D, 0x00, 0x00, 0x2B, 0xFE, 0x93, 0xEF};
static volatile uint8_t receiveBufferIndex = 0;
static volatile uint8_t voiceOverBuffer[20];
static void (*audioCompleteCallback)(void);
static dmac_descriptor_registers_t dmaDescriptor[1] __attribute__((aligned(16)));
static dmac_descriptor_registers_t dmaWriteBack[1] __attribute__((aligned(16)));

extern "C" void TC3_Handler(void)
{
    TC3_REGS->COUNT16.TC_COUNT = FIXED_UPDATE_TIMER_PRELOAD_VALUE;
    TC3_REGS->COUNT16.TC_INTFLAG = 0x01;
    DeviceUtility::fixedUpdate = true;
}

extern "C" void SERCOM5_Handler(void)
{
    if (SERCOM5_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_RXC_Msk)
    {
        uint8_t data = SERCOM5_REGS->USART_INT.SERCOM_DATA;
        voiceOverBuffer[receiveBufferIndex] = data;
        receiveBufferIndex++;

        if (receiveBufferIndex == 20 && data == 0xEF)
        {
            if (memcmp((const void *)voiceOverBuffer, MUTE_COMMAND_RESPONSE, sizeof(voiceOverBuffer)) == 0)
            {
                DeviceUtility::Instance().processAudioCommand(DFPlayerCommand::Mute);
            }

            receiveBufferIndex = 0;

            if (audioCompleteCallback != nullptr)
            {
                audioCompleteCallback();
            }
        }

        SERCOM5_REGS->USART_INT.SERCOM_INTFLAG = SERCOM_USART_INT_INTFLAG_RXC_Msk;
    }
}

SAMD21J18Utility::SAMD21J18Utility()
    : DeviceUtility(),
      rowOneBaseAddress(0x001),
      rowTwoBaseAddress(0x0010),
      rowThreeBaseAddress(0x0100),
      rowFourBaseAddress(0x1000),
      currentLedIndex(0),
      ledColorData{}
{
}

void SAMD21J18Utility::configure()
{
    configureFixedUpdateTimer();
    configureButtonPins();
    configureLeds();
    configureRNG();
    configureAudio();
    configureAudioDMAC();
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

void SAMD21J18Utility::refreshButtonColor(volatile uint16_t ledColorData[4][4][8])
{
    LED *led = &LED::leds[currentLedIndex];
    PORT_REGS->GROUP[1].PORT_OUT = (PORT_REGS->GROUP[1].PORT_OUT & 0xFFFFFFF0) | (1 << led->column);

    for (int colorDataIndex = 0; colorDataIndex < 8; ++colorDataIndex)
    {
        transmitLedData(ledColorData[led->column][led->row][colorDataIndex]);
        latchLedData();

        for (int j = 0; j < (1 << colorDataIndex); ++j)
        {
            __NOP();
        }
    }

    transmitLedData(0x0000);
    latchLedData();

    ++currentLedIndex;

    if (currentLedIndex > 15)
        currentLedIndex = 0;
}

void SAMD21J18Utility::processAudioCommand(DFPlayerCommand command, void (*callback)(void))
{
    audioCompleteCallback = callback;
    const uint8_t *data = DF_PLAYER_COMMANDS[command];

    // Select channel 0
    DMAC_REGS->DMAC_CHID = DMA_CH_UART_TX;

    // Disable the channel
    DMAC_REGS->DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE_Msk;

    // Update descriptor
    dmaDescriptor[0].DMAC_BTCNT = 10;
    dmaDescriptor[0].DMAC_SRCADDR = (uint32_t)(data + 10);
    dmaDescriptor[0].DMAC_DSTADDR = (uint32_t)&SERCOM5_REGS->USART_INT.SERCOM_DATA;
    dmaDescriptor[0].DMAC_DESCADDR = 0; // no next descriptor

    // Enable channel
    DMAC_REGS->DMAC_CHCTRLA |= DMAC_CHCTRLA_ENABLE_Msk;
}

void SAMD21J18Utility::setBeepNote(MusicNote note)
{
    TC7_REGS->COUNT16.TC_CC[0] = static_cast<uint16_t>(note);
    TC7_REGS->COUNT16.TC_CC[1] = static_cast<uint16_t>(note) / 2;
    TC7_REGS->COUNT16.TC_COUNT = 0;
}

void SAMD21J18Utility::enableBeep(bool enable)
{
    if (enable)
    {
        TC7_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    }
    else
    {
        TC7_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    }

    while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk)
    {
    }
}

uint16_t SAMD21J18Utility::getRNGSeedValue()
{
    TC5_REGS->COUNT16.TC_COUNT;
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
    while ((TC3_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk)
    {
    }
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

void SAMD21J18Utility::configureLeds()
{
    // Configure outputs for leds
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB00 | PORT_PB01 | PORT_PB02 | PORT_PB03;
    PORT_REGS->GROUP[1].PORT_OUT &= ~(PORT_PB00 | PORT_PB01 | PORT_PB02 | PORT_PB03);

    // Configure spi for communication with led driver
    // Enable gpio multiplexer for pb12 and pb13.
    PORT_REGS->GROUP[1].PORT_PINCFG[12] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[1].PORT_PINCFG[13] = PORT_PINCFG_PMUXEN_Msk;

    // Select peripheral function C for pb12, pb13. pb12 will be Dout and pb 13 will be the clock
    PORT_REGS->GROUP[1].PORT_PMUX[6] = PORT_PMUX_PMUXO_C | PORT_PMUX_PMUXE_C;

    // configure pb14 to be an output. Will control the latch in software
    PORT_REGS->GROUP[1].PORT_DIR |= PORT_PB14;
    PORT_REGS->GROUP[1].PORT_OUT &= ~PORT_PB14;

    // Turn on clock for SERCOM4
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM4_Msk;

    // Configure sercom 4 to use clock generator 0
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM4_CORE | GCLK_CLKCTRL_CLKEN_Msk;

    // Gives us a baud rate of 1Mhz
    SERCOM4_REGS->SPIM.SERCOM_BAUD = 2;

    // Enable hardware SS control. Character sizse is set to 8 bits
    SERCOM4_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_CHSIZE_8_BIT;
    while ((SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_CTRLB_Msk) == SERCOM_SPIM_SYNCBUSY_CTRLB_Msk)
    {
    }

    // Set sercom 4 to spi master. Transmit msb first. Set to SPI frame. Configure pads.
    SERCOM4_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_ENABLE_Msk | SERCOM_SPIM_CTRLA_MODE_SPI_MASTER | SERCOM_SPIM_CTRLA_DORD_MSB | SERCOM_SPIM_CTRLA_FORM_SPI_FRAME | SERCOM_SPIM_CTRLA_DIPO_PAD3 | SERCOM_SPIM_CTRLA_DOPO_PAD0 | SERCOM_SPIM_CTRLA_CPOL_IDLE_HIGH;
    while ((SERCOM4_REGS->SPIM.SERCOM_SYNCBUSY & SERCOM_SPIM_SYNCBUSY_ENABLE_Msk) == SERCOM_SPIM_SYNCBUSY_ENABLE_Msk)
    {
    }
}

void SAMD21J18Utility::configureAudio()
{
    // Configure PWM for music tones
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC7_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC6_TC7 | GCLK_CLKCTRL_CLKEN_Msk;

    PORT_REGS->GROUP[1].PORT_PINCFG[23] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[1].PORT_PMUX[11] = PORT_PMUX_PMUXO_E;

    TC7_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MPWM | TC_CTRLA_PRESCALER_DIV256;
    while ((TC7_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk)
    {
    }

    // // Configure SERCOM5 USart for DFPlayer mini
    // Turn on clock for usart
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_SERCOM5_Msk;

    // Configure clocks for timer and interrupts
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM5_CORE | GCLK_CLKCTRL_CLKEN_Msk;

    // Enable pin multiplexer for PA20 and configure it to be function C Pad 0
    PORT_REGS->GROUP[0].PORT_PINCFG[20] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[0].PORT_PMUX[10] |= PORT_PMUX_PMUXE_C;

    // Enable pin multiplexer for PA21 and configure it to be function C Pad 0
    PORT_REGS->GROUP[0].PORT_PINCFG[21] = PORT_PINCFG_PMUXEN_Msk;
    PORT_REGS->GROUP[0].PORT_PMUX[10] |= PORT_PMUX_PMUXO_C;

    SERCOM5_REGS->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_TXEN_Msk | SERCOM_USART_INT_CTRLB_RXEN_Msk;
    while ((SERCOM5_REGS->USART_INT.SERCOM_SYNCBUSY) != 0U)
    {
    }

    // Sets the baud rate to 9600
    SERCOM5_REGS->USART_INT.SERCOM_BAUD = 0xFF2E;
    SERCOM5_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_RXC_Msk;
    SERCOM5_REGS->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_ENABLE_Msk | SERCOM_USART_INT_CTRLA_DORD_LSB | SERCOM_USART_INT_CTRLA_TXPO(1) | SERCOM_USART_INT_CTRLA_RXPO(3) | SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK;
    NVIC_EnableIRQ(SERCOM5_IRQn);
}

void SAMD21J18Utility::configureRNG()
{
    // Timer 5 is configured in normal mode with no. Overflow interrupt will be triggered every .22222ms.
    // Power Manager
    PM_REGS->PM_APBCMASK |= PM_APBCMASK_TC5_Msk;

    // Generic Clock Controller
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5 | GCLK_CLKCTRL_CLKEN_Msk;

    // 8 bit timer configuration
    TC5_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk;
    TC5_REGS->COUNT16.TC_CTRLA = TC_CTRLA_ENABLE_Msk | TC_CTRLA_MODE_COUNT8 | TC_CTRLA_WAVEGEN_NFRQ;
    while ((TC5_REGS->COUNT16.TC_STATUS & TC_STATUS_SYNCBUSY_Msk) == TC_STATUS_SYNCBUSY_Msk)
    {
    }
}

void SAMD21J18Utility::configureAudioDMAC()
{
    // Enable APB clock for DMAC
    PM_REGS->PM_AHBMASK |= PM_AHBMASK_DMAC_Msk;
    PM_REGS->PM_APBBMASK |= PM_APBBMASK_DMAC_Msk;

    // Reset DMAC
    DMAC_REGS->DMAC_CTRL = DMAC_CTRL_SWRST_Msk;

    // Set base addresses for descriptor and writeback areas
    DMAC_REGS->DMAC_BASEADDR = (uint32_t)dmaDescriptor;
    DMAC_REGS->DMAC_WRBADDR = (uint32_t)dmaWriteBack;

    // Enable DMAC
    DMAC_REGS->DMAC_CTRL = DMAC_CTRL_DMAENABLE_Msk | DMAC_CTRL_LVLEN(0xF);

    // Select channel 0
    DMAC_REGS->DMAC_CHID = 0;

    // Configure channel control
    DMAC_REGS->DMAC_CHCTRLA = 0; // disable before config

    // trigger on SERCOM5 TX one beat per trigger
    DMAC_REGS->DMAC_CHCTRLB = DMAC_CHCTRLB_TRIGSRC(SERCOM5_DMAC_ID_TX) | DMAC_CHCTRLB_TRIGACT_BEAT;

    // Setup descriptor
    dmaDescriptor[0].DMAC_BTCTRL = DMAC_BTCTRL_VALID_Msk | DMAC_BTCTRL_SRCINC_Msk | DMAC_BTCTRL_BEATSIZE_BYTE;
}

void SAMD21J18Utility::transmitLedData(uint16_t data)
{
    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk))
    {
    }
    SERCOM4_REGS->SPIM.SERCOM_DATA = data >> 8;

    while (!(SERCOM4_REGS->SPIM.SERCOM_INTFLAG & SERCOM_SPIM_INTFLAG_DRE_Msk))
    {
    }
    SERCOM4_REGS->SPIM.SERCOM_DATA = data & 0xFF;
}

void SAMD21J18Utility::latchLedData()
{
    PORT_REGS->GROUP[1].PORT_OUT |= PORT_PB14;
    PORT_REGS->GROUP[1].PORT_OUT &= ~PORT_PB14;
}