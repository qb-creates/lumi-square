#include "shutdownutility.h"
#include "audio.h"
#include "buttons.h"
#include "random.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR(PCINT0_vect)
{
    ShutdownUtility::Instance().resetShutdownTimer();
}

ISR(PCINT3_vect)
{
    ShutdownUtility::Instance().resetShutdownTimer();
}

ShutdownUtility::ShutdownUtility()
    : FixedUpdateEventListener(), m_shutdownTimer(0), m_shutdownTime(300000)

{
    // Enable Pin control interrupts PC4 - PC7
    PCICR |= _BV(PCIE3) | _BV(PCIE0);
    PCMSK0 |= _BV(PCINT4) | _BV(PCINT5) | _BV(PCINT6) | _BV(PCINT7);
    PCMSK3 |= _BV(PCINT26) | _BV(PCINT27);

    DDRC |= _BV(PC3);
    PORTC &= ~_BV(PC3);
}

ShutdownUtility &ShutdownUtility::Instance()
{
    static ShutdownUtility instance;
    return instance;
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
void ShutdownUtility::setShutdownTimeout(uint32_t timeout)
{
    m_shutdownTime = timeout;
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
void ShutdownUtility::updateShutdownTimer()
{
    m_shutdownTimer += FixedUpdateTimer::DELTA_TIME;

    if (m_shutdownTimer >= m_shutdownTime)
    {
        PORTC |= _BV(PC3);
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
void ShutdownUtility::resetShutdownTimer()
{
    m_shutdownTimer = 0;
}

void ShutdownUtility::onFixedUpdate()
{
    updateShutdownTimer();
}
