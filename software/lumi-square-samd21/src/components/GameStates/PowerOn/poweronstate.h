#ifndef POWERON_H
#define POWERON_H

#include "../basestate.h"
#include "../../Colors/colors.h"
class PowerOnState : public GameBaseState
{
public:
    PowerOnState();
    void enterState(GameState previousState);
    void exitState();
    void updateState();
    void onButtonPressed(int8_t buttonIndex);

private:
    const uint8_t m_animationTransitionTime;
    const uint8_t m_nextStateDelayTime;
    uint16_t m_animationTimer;
    int16_t m_nextStateDelayTimer;
    bool m_reverseAnimation;
    bool m_animationComplete;
    uint8_t m_currentLedIndex;
    int8_t m_buttonIndexList[16];
    Color m_ledColorList[16];
};

#endif
