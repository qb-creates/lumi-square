#ifndef SLEEPMANAGER_H
#define SLEEPMANAGER_H

#include <stdint.h>
#include "fixedupdate.h"

class ShutdownUtility : public FixedUpdateEventListener
{
public:
    static ShutdownUtility& Instance();
    void setShutdownTimeout(uint32_t timeout);
    void resetShutdownTimer();
    void onFixedUpdate() override;

private:
    volatile uint32_t m_shutdownTimer;
    uint32_t m_shutdownTime;

private:
    ShutdownUtility();
    ShutdownUtility(const ShutdownUtility &) = delete;
    void operator=(const ShutdownUtility &) = delete;
    void updateShutdownTimer();
};

#endif