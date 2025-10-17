#ifndef DEVICEUTILITY_H
#define DEVICEUTILITY_H

#include <stdint.h>

#define FIXED_UPDATE_INTERVAL_MS 16
#define FIXED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - ((F_CPU / 1024UL) * FIXED_UPDATE_INTERVAL_MS) / 1000UL)

#define LED_UPDATE_INTERVAL_US 21845
#define LED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - (uint32_t)((((uint64_t)(F_CPU) / 16UL) * LED_UPDATE_INTERVAL_US) / 1000000ULL))

class DeviceUtility
{

public:
    static DeviceUtility &Instance();
    static const int8_t DELTA_TIME;
    static volatile bool fixedUpdate;

    virtual void configureFixedUpdateTimer() = 0;
    /**
     * @brief Configures pins for system buttons and button matrix.
     *
     * This function configures PC4 and PC5 as inputs for the next and previous buttons, respectively.
     * Additionally, it configures DDRA as inputs and outputs for the button matrix.
     *
     * @return void
     */
    virtual void configureButtonPins() = 0;
    virtual bool getPreviousButtonState() = 0;
    virtual bool getNextButtonState() = 0;
    virtual bool getDifficultyButtonState() = 0;

    /**
     * @brief Scans the button matrix to detect pressed buttons.
     *
     * This function scans a button matrix connected to the microcontroller,
     * detecting if any buttons have been pressed. It iterates through the rows
     * and columns of the matrix to determine the status of each button.
     *
     * It is important to call this function frequently to ensure proper
     * detection of button presses.
     *
     * @return void
     */
    virtual uint32_t scanButtonMatrix() = 0;

public:
    DeviceUtility();

private:
    DeviceUtility(const DeviceUtility &) = delete;
    void operator=(const DeviceUtility &) = delete;
};

#endif