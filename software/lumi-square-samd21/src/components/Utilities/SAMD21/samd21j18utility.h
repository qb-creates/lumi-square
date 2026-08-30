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
    volatile uint8_t currentLedIndex;
    volatile uint16_t ledColorData[4][4][8];

public:
    SAMD21J18Utility();
    void configure() override;
    bool getPreviousButtonState() override;
    bool getNextButtonState() override;
    bool getDifficultyButtonState() override;
    uint32_t scanButtonMatrix() override;
    void refreshButtonColor(volatile uint16_t ledColorData[4][4][8]) override;
    void processAudioCommand(DFPlayerCommand command, void (*callback)(void) = nullptr) override;
    void setBeepNote(MusicNote note) override;
    void enableBeep(bool enable) override;
    uint16_t getRNGSeedValue() override;

private:
    void configureFixedUpdateTimer() override;
    void configureButtonPins() override;
    void configureLeds() override;
    void configureAudio() override;
    void configureRNG() override;
    void configureAudioDMAC();
    void transmitLedData(uint16_t data);
    void latchLedData(void);
};

#endif