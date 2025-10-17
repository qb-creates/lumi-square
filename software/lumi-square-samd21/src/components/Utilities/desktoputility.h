#ifndef DESKTOPUTILITY_H
#define DESKTOPUTILITY_H

#include "deviceutility.h"

class DesktopUtility : public DeviceUtility
{
public:
    DesktopUtility();
    void configureButtonPins() override;
    bool getPreviousButtonState() override;
    bool getNextButtonState() override;
    bool getDifficultyButtonState() override;
    uint32_t scanButtonMatrix() override;
};

#endif