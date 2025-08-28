/**
 * \file
 *
 * \brief System configuration file for ATSAMD21J18A
 *
 * Copyright (c) 2022 Microchip Technology Inc.
 *
 * \license_start
 *
 * \page License
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \license_stop
 *
 */

#include "samd21j18a.h"
#include "system_samd21.h"
/** \cond 0 */
/* *INDENT-OFF* */
#ifdef __cplusplus
extern "C"
{
#endif
/* *INDENT-ON* */
/** \endcond */

/**
 * Initial system clock frequency. The System RC Oscillator (RCSYS) provides
 *  the source for the main clock at chip startup.
 */
#define __SYSTEM_CLOCK (48000000)

    uint32_t SystemCoreClock = __SYSTEM_CLOCK; /*!< System Clock Frequency (Core Clock)*/
    
    void initializeDFLL(void);
    void initializeClockGenerator_0(void);
    void configureSysTick(void);

    /**
     * Initialize the system
     *
     * \brief  Setup the microcontroller system.
     *         Initialize the System and update the SystemCoreClock variable.
     */
    void SystemInit(void)
    {
        // Keep the default device state after reset
        SystemCoreClock = __SYSTEM_CLOCK;
        NVMCTRL_REGS->NVMCTRL_CTRLB = NVMCTRL_CTRLB_RWS(3UL);
        initializeDFLL();
        initializeClockGenerator_0();
        configureSysTick();
        __DMB();
        __enable_irq();
    }

    /**
     * Update SystemCoreClock variable
     *
     * \brief  Updates the SystemCoreClock with current core Clock
     *         retrieved from cpu registers.
     */
    void SystemCoreClockUpdate(void)
    {
        // Not implemented
        SystemCoreClock = __SYSTEM_CLOCK;
        return;
    }

    void initializeDFLL(void)
    {
        SYSCTRL_REGS->SYSCTRL_DFLLCTRL &= ~SYSCTRL_DFLLCTRL_ONDEMAND_Msk;

        while ((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk)
        {
        }

        // Coarse value is grabbed from memory location 0x00806021U. It will be the last 6 bits. If it is equal to 0x3F then set calibCoarse to 0x1F.
        // Default coarse value seems to b 0x1B. Datasheet says set fine to 512.
        uint32_t calibCoarse = ((*(uint32_t *)0x00806024U >> 26U) & 0x3FU);
        calibCoarse = calibCoarse == 0x3FU ? 0x1FU : calibCoarse;

        SYSCTRL_REGS->SYSCTRL_DFLLVAL = SYSCTRL_DFLLVAL_COARSE(calibCoarse) | SYSCTRL_DFLLVAL_FINE(512U);

        while ((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk)
        {
        }

        SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_ENABLE_Msk;

        while ((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk)
        {
        }

        // Power Manager
        PM_REGS->PM_APBASEL = PM_APBASEL_APBADIV_DIV1;
        PM_REGS->PM_APBBSEL = PM_APBASEL_APBADIV_DIV1;
        PM_REGS->PM_APBCSEL = PM_APBCSEL_APBCDIV_DIV1;
    }

    void initializeClockGenerator_0(void)
    {
        // Enable generic clock generator 0 and select DFLL48M
        GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(0);

        /* wait for the Generator 0 synchronization */
        while ((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
        {
        }

        /*Disable RC oscillator*/
        SYSCTRL_REGS->SYSCTRL_OSC8M = 0x0U;
        SYSCTRL_REGS->SYSCTRL_OSC32K = 0x0U;
    }

    void initializeXOSC32K(void)
    {
        SYSCTRL_REGS->SYSCTRL_XOSC32K = SYSCTRL_XOSC32K_WRTLOCK_Msk | SYSCTRL_XOSC_XTALEN_Msk | SYSCTRL_XOSC32K_AAMPEN_Msk | SYSCTRL_XOSC32K_EN32K_Msk | SYSCTRL_XOSC32K_ENABLE_Msk;

        GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(1);

        /* wait for the Generator 1 synchronization */
        while ((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
        {
        }

        // Uncomment and enable output on GENCTRL to see output on PB15
        // PORT_REGS->GROUP[1].PORT_PINCFG[15] = PORT_PINCFG_PMUXEN_Msk;
        // PORT_REGS->GROUP[1].PORT_PMUX[7] = PORT_PMUX_PMUXO_H;
    }

    /**
     * @brief Sets OSCULP32k to be on clock generator 1
     *
     */
    void initializeOSCULP32K(void)
    {
        // SYSCTRL_REGS->SYSCTRL_OSCULP32K = SYSCTRL_OSCULP32K_WRTLOCK_Msk | SYSCTRL_OSCULP32K_CALIB(31);
        GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(1) | GCLK_GENCTRL_DIVSEL_DIV2;
        GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(1);

        /* wait for the Generator 1 synchronization */
        while ((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
        {
        }

        // // Uncomment and enable output on GENCTRL to see output on PB15
        // PORT_REGS->GROUP[1].PORT_PINCFG[15] = PORT_PINCFG_PMUXEN_Msk;
        // PORT_REGS->GROUP[1].PORT_PMUX[7] = PORT_PMUX_PMUXO_H;
    }

    void configureSysTick(void)
    {
        // Set the priority of the SysTick interrupt
        NVIC_SetPriority(SysTick_IRQn, 3);

        // Load the SysTick counter value for 1 millisecond interrupts
        SysTick->LOAD = (48000000 / 1000) - 1;

        // Reset the SysTick counter value
        SysTick->VAL = 0;

        // Enable SysTick with the system clock and interrupt
        SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
        // SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    }

/** \cond 0 */
/* *INDENT-OFF* */
#ifdef __cplusplus
}
#endif
/* *INDENT-ON* */
/** \endcond */
