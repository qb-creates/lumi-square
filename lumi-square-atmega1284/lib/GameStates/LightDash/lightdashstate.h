#ifndef LIGHTDASH_H
#define LIGHTDASH_H

#include "basestate.h"
#include "colors.h"

class LightDashState : public GameBaseState
{
public:
    LightDashState();
    void enterState(GameState previousState);
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    int16_t gameTimer;
    int8_t remainingGameTime;
    int8_t scoreDeductionAmount;
    int8_t bonusLedPointValue;
    int16_t maxLedTurnOffTime;
    int16_t minLedTurnOffTime;
    int16_t bonusLedTurnOffTime;
    bool enablePowerUps;
    bool bonusLedActive;
    int16_t ledTurnOffTimers[16];
    void updateLedOffTimers();
    void updateGameTimer();
    void turnOnRandomLed();
    void turnOffSelectedLed(int8_t buttonIndex);
    void deductPointsFromScore();
    void addPointsToScore(int8_t buttonIndex);
    void playMusicNote(int8_t buttonIndex);
};

#endif