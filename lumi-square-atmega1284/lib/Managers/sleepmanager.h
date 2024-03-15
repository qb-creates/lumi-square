#ifndef SLEEPMANAGER_H
#define SLEEPMANAGER_H

#include <stdint.h>
#include "fixedupdate.h"
class SleepManager : public FixedUpdateEventListener
{
public:
    static SleepManager& Instance();
    bool isSleep();
    void setSleepTimeout(uint16_t timeout);
    void resetSleepTimer();
    void wakeUpInterruptHandler();
    void onFixedUpdate() override;

private:
    volatile bool m_isSleep;
    volatile uint16_t m_sleepTimer;
    int16_t m_sleepButtonTimer;
    uint16_t m_sleepTimeout;
    bool m_enableSleep;

private:
    SleepManager();
    SleepManager(const SleepManager &) = delete;
    void operator=(const SleepManager &) = delete;
    void updateSleepTimer();
    void enterSleepMode();
};

#endif