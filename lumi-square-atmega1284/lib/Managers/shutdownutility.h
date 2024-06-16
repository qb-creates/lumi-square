#ifndef SLEEPMANAGER_H
#define SLEEPMANAGER_H

#include <stdint.h>
#include "fixedupdate.h"

class ShutdownUtility : public FixedUpdateEventListener
{
public:
    static ShutdownUtility& Instance();
    void setShutdownTimeout(uint16_t timeout);
    void resetShutdownTimer();
    void onFixedUpdate() override;

private:
    volatile uint16_t m_sleepTimer;
    uint16_t m_sleepTimeout;

private:
    ShutdownUtility();
    ShutdownUtility(const ShutdownUtility &) = delete;
    void operator=(const ShutdownUtility &) = delete;
    void updateShutdownTimer();
};

#endif