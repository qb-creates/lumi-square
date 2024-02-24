#ifndef LIGHTDASH_H
#define LIGHTDASH_H

#include "basestate.h"

class LightDashState : public GameBaseState
{
public:
    LightDashState();
    void enterState();
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    int16_t timer;
    int8_t counter;
    int16_t score;
    bool shuffleLeds;
    bool enablePowerUps;
    bool timePowerUpActive;
    int16_t ledShuffleTimes[16];
    void powerOnRandomLight();
};

#endif