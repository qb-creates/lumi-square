#ifndef DESKTOPUTILITY_H
#define DESKTOPUTILITY_H

#include "deviceutility.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <miniaudio.h>

class DesktopUtility : public DeviceUtility
{
private:
    GLFWwindow *window;
    uint32_t buttonData;
    bool nextButtonState;
    bool previousButtonState;
    bool difficultyButtonState;

public:
    DesktopUtility();
    ~DesktopUtility();
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
};

#endif