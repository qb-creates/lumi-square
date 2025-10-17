#ifndef SAMD21J18UTILITY_H
#define SAMD21J18UTILITY_H

#include "deviceutility.h"

class SAMD21J18Utility : public DeviceUtility
{

private:
    const uint32_t rowOneBaseAddress;
    const uint32_t rowTwoBaseAddress;
    const uint32_t rowThreeBaseAddress;
    const uint32_t rowFourBaseAddress;

public:
    SAMD21J18Utility();
    void configureFixedUpdateTimer() override;
    void configureButtonPins() override;
    bool getPreviousButtonState() override;
    bool getNextButtonState() override;
    bool getDifficultyButtonState() override;
    uint32_t scanButtonMatrix() override;

};

#endif