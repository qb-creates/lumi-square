#ifndef FIXEDUPDATETIMER_H
#define FIXEDUPDATETIMER_H

#include <stdint.h>

class FixedUpdateEventListener
{
public:
    virtual void onFixedUpdate() = 0;
};

class FixedUpdateTimer
{
public:
    static const int8_t DELTA_TIME;
    static volatile bool fixedUpdate;
    static void enableFixedUpdate();
    static void disableFixedUpdate();

private:
    FixedUpdateTimer() = delete;
    FixedUpdateTimer(const FixedUpdateTimer &) = delete;
    void operator = (const FixedUpdateTimer &) = delete;    
    static const int8_t MAX_HANDLERS;
};
#endif
