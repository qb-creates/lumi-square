#include "deviceutility.h"

#ifdef DESKTOP_SIMULATION
    #include "desktoputility.h"
#else
    #include "samd21j18utility.h"
#endif

const int8_t DeviceUtility::DELTA_TIME = FIXED_UPDATE_INTERVAL_MS;
volatile bool DeviceUtility::fixedUpdate = false;

DeviceUtility::DeviceUtility()
{
}

DeviceUtility &DeviceUtility::Instance()
{
#ifdef DESKTOP_SIMULATION
    static DesktopUtility instance;
#else
    static SAMD21J18Utility instance;
#endif
    return instance;
}

