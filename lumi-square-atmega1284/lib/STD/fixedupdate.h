#ifndef FIXEDUPDATETIMER_H
#define FIXEDUPDATETIMER_H

#include <stdint.h>

class FixedUpdateEventListener {
public:
    virtual void onFixedUpdate() = 0;
};

class FixedUpdateTimer
{
public:
    static volatile bool fixedUpdate;
    static volatile uint8_t fixedUpdateCounter;
    static void configureFixedUpdate();
    static void registerEventHandler(FixedUpdateEventListener* listener);
    static void triggerFixedUpdateEvent();

private:
    static int numHandlers;
    static const int MAX_HANDLERS;    
    static FixedUpdateEventListener *eventListeners[10];
};
#endif
