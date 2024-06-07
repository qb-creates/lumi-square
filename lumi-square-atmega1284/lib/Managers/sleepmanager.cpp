#include "sleepmanager.h"
#include "audio.h"
#include "buttons.h"
#include "lcd.h"
#include "random.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR(PCINT0_vect)
{
    if (SleepManager::Instance().isSleep())
    {
        SleepManager::Instance().wakeUpInterruptHandler();
        return;
    }
    
    SleepManager::Instance().resetSleepTimer();
}

ISR(PCINT3_vect)
{
    if (SleepManager::Instance().isSleep())
    {
        SleepManager::Instance().wakeUpInterruptHandler();
        return;
    }

    SleepManager::Instance().resetSleepTimer();
}

SleepManager::SleepManager()
    : FixedUpdateEventListener(), m_isSleep(false), m_sleepTimer(0), m_sleepTimeout(45000)

{
    // Enable Pin control interrupts PC4 - PC7
    PCICR = _BV(PCIE3) | _BV(PCIE0);
    PCMSK0 = _BV(PCINT4) | _BV(PCINT5) | _BV(PCINT6) | _BV(PCINT7);
    PCMSK3 = _BV(PCINT26) | _BV(PCINT27);

    // Set the sleep mode to PWR_DOWN.
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

SleepManager &SleepManager::Instance()
{
    static SleepManager instance;
    return instance;
}

/**
 * @brief Checks if the microcontroller is in sleep mode.
 *
 * This function returns a boolean value indicating whether the microcontroller
 * is currently in sleep mode.
 *
 * @return bool True if the microcontroller is in sleep mode, false otherwise.
 */
bool SleepManager::isSleep()
{
    return m_isSleep;
}

/**
 * @brief Sets the time duration for the microcontroller to go to sleep.
 *
 * This function allows you to specify the time duration in milliseconds
 * after which the microcontroller will automatically go to sleep. Once
 * the specified time elapses, the microcontroller will enter sleep mode.
 *
 * @param timeout The time duration in milliseconds for the sleep timeout.
 *
 * @return void
 */
void SleepManager::setSleepTimeout(uint16_t timeout)
{
    m_sleepTimeout = timeout;
}

/**
 * @brief Updates the sleep timer by adding a specified duration.
 *
 * This function updates the sleep timer by adding the specified duration
 * in milliseconds every time it is called. When the accumulated time
 * reaches the timeout set by setSleepTimeout(), the microcontroller
 * will go to sleep.
 *
 * @param time The duration to add to the sleep timer in milliseconds.
 *
 * @return void
 */
void SleepManager::updateSleepTimer()
{
    m_sleepTimer += FixedUpdateTimer::DELTA_TIME;

    if (m_sleepTimer >= m_sleepTimeout)
    {
        enterSleepMode();
    }
}

/**
 * @brief Resets the sleep timer to zero.
 *
 * This function resets the sleep timer, effectively setting the accumulated time
 * back to zero. It can be used when you want to restart the timer for
 * determining when the microcontroller should go to sleep.
 *
 * @return void
 */
void SleepManager::resetSleepTimer()
{
    m_sleepTimer = 0;
}

/**
 * @brief Puts the microcontroller into sleep mode to conserve power.
 *
 * This function puts the microcontroller into a low-power sleep mode,
 * suspending its operation until a wake-up event occurs. External interrupts
 * are enabled, and PD2 (Pin D2) is configured to cause the microcontroller
 * to wake up from sleep mode.
 *
 * @return void
 */
void SleepManager::enterSleepMode()
{
    m_sleepTimer = 0;
    m_isSleep = true;
    Random::seedRNG();
    LCD::Instance().displayPower(false);    
    PORTA = ~(0x0F); // Enable All led buttons
    sleep_mode();
}

/**
 * @brief Interrupt handler for waking up from sleep mode.
 *
 * This function is called when the microcontroller wakes up from sleep mode
 * due to an external interrupt on Pin D2. It performs specific actions that
 * need to be executed upon waking up, such as disabling external interrupts
 * and powering on the LCD.
 *
 * @return void
 */
void SleepManager::wakeUpInterruptHandler()
{
    m_isSleep = false;
    LCD::Instance().displayPower(true);
}

void SleepManager::onFixedUpdate()
{
    updateSleepTimer();
}
