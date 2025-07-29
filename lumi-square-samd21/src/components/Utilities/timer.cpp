#include "timer.h"

Timer::Timer(uint16_t miliSeconds) : targetTime(miliSeconds), currentTime(0), timerComplete(false) {}

void Timer::updateTimer(uint8_t timePassed)
{
    if (timerComplete)
        return;

    currentTime += timePassed;

    if (currentTime >= targetTime)
    {
        timerComplete = true;
    }
}

void Timer::setTargetTime(uint16_t miliSeconds)
{
    targetTime = miliSeconds;
    resetTimer();
}

void Timer::resetTimer()
{
    timerComplete = false;
    currentTime = 0;
}

uint16_t Timer::remaingTime()
{
    return (targetTime - currentTime) / 1000;
}

bool Timer::isComplete()
{
    return timerComplete;
}
