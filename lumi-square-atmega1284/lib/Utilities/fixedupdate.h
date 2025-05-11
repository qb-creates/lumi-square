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
    static void configureFixedUpdate();
    static void disableFixedUpdate();
    static void registerEventHandler(FixedUpdateEventListener *listener);
    static void triggerFixedUpdateEvent();

private:
    FixedUpdateTimer() = delete;
    FixedUpdateTimer(const FixedUpdateTimer &) = delete;
    void operator = (const FixedUpdateTimer &) = delete;
    static int8_t numHandlers;
    static const int8_t MAX_HANDLERS;
    static FixedUpdateEventListener *eventListeners[10];
};
#endif
