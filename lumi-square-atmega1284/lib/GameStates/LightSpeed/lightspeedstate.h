#ifndef LIGHTSPEED_H
#define LIGHTSPEED_H

#include "basestate.h"

class LightSpeedState : public GameBaseState
{
public:
    LightSpeedState();
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
    bool redHazardActive;
    int16_t ledShuffleTimes[16];
    void powerOnRandomLight();
};

#endif