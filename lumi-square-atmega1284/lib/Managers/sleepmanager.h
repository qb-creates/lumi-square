#ifndef SLEEPMANAGER_H
#define SLEEPMANAGER_H

#include <stdint.h>

class SleepManager
{
public:
    static bool isSleep();
    static void setSleepTimeout(uint16_t timeout);
    static void updateSleepTimer(uint16_t time);
    static void resetSleepTimer();
    static void enterSleepMode();
    static void wakeUpInterruptHandler();

private:
    static volatile bool m_isSleep;
    static uint16_t m_sleepTimer;
    static uint16_t m_sleepTimeout;

private:
    SleepManager(const SleepManager &) = delete;
    void operator=(const SleepManager &) = delete;
};

#endif