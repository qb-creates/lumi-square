#ifndef TIMER_H
#define TIMER_H

#include "stdint.h"

class Timer
{
    private:
        uint16_t targetTime;
        uint16_t currentTime;
        bool timerComplete;

    public:
        Timer(uint16_t miliSeconds);
        void updateTimer(uint8_t timePassed);
        void setTargetTime(uint16_t miliSeconds);
        void resetTimer();
        uint16_t remaingTime();
        bool isComplete();
};
#endif