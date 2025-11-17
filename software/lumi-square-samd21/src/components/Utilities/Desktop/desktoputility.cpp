#include "desktoputility.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>
#include <led.h>
#include <random>
#include <sstream>
#include <thread>

#define MA_ENABLE_MP3

// Atomic flag to control the thread
std::atomic<bool> running{true};

// Audio variables
static void (*audioCompleteCallback)(void);
static ma_engine audioEngine;
static ma_sound audioSound;
static ma_result audioInitResult;
static ma_device device;

static float g_freq = 0.0f;      // Frequency in Hz
static float g_phase = 0.0f;     // Phase accumulator
static int g_sampleRate = 48000; // Default (updated by device)

// Function that will run in the thread
void fixedUpdateThreadFunction()
{
    using namespace std::chrono;
    auto nextTime = steady_clock::now();

    while (running)
    {
        nextTime += milliseconds(DeviceUtility::DELTA_TIME);

        // Signal fixedUpdate
        if (!DeviceUtility::fixedUpdate)
            DeviceUtility::fixedUpdate = true;

        std::this_thread::sleep_until(nextTime);
    }
}

void audioCallback(ma_device *device, void *output, const void *input, ma_uint32 frameCount)
{
    float *out = (float *)output;

    float sr = (float)device->sampleRate;
    g_sampleRate = device->sampleRate;

    float phaseInc = (2.0f * 3.1415926f * g_freq) / sr;

    for (uint32_t i = 0; i < frameCount; i++)
    {

        float sample = 0.0f;

        if (g_freq > 0.0f)
        {
            // Square wave: +0.2 or -0.2 depending on sign of sin()
            sample = (sinf(g_phase) > 0.0f ? 0.2f : -0.2f);
            g_phase += phaseInc;
            if (g_phase >= 2.0f * 3.1415926f)
                g_phase -= 2.0f * 3.1415926f;
        }

        // Stereo
        out[i * 2 + 0] = sample;
        out[i * 2 + 1] = sample;
    }
}

void setTone(float freq)
{
    // 0 = off
    g_freq = freq;

    if (freq == 0)
        g_phase = 0; // reset phase so next beep is clean
}

void onMiniaudioComplete(void *pUserData, ma_sound *pSound)
{
    if (audioCompleteCallback != nullptr)
    {
        audioCompleteCallback();
    }
}

DesktopUtility::DesktopUtility()
    : DeviceUtility(),
      window(),
      nextButtonState(false),
      previousButtonState(false),
      difficultyButtonState(false),
      queueMusicNote(MusicNote::A0)
{
}

DesktopUtility::~DesktopUtility()
{
    if (audioInitResult == MA_SUCCESS)
    {
        ma_engine_uninit(&audioEngine);
    }
}

void DesktopUtility::configure()
{
    configureFixedUpdateTimer();
    configureButtonPins();
    configureLeds();
    configureAudio();
    configureRNG();
}

bool DesktopUtility::getPreviousButtonState()
{
    return previousButtonState;
}

bool DesktopUtility::getNextButtonState()
{
    return nextButtonState;
}

bool DesktopUtility::getDifficultyButtonState()
{
    return difficultyButtonState;
}

uint32_t DesktopUtility::scanButtonMatrix()
{
    return buttonData;
}

void DesktopUtility::refreshButtonColor(volatile uint16_t ledColorData[4][4][8])
{

    bool button_states[4][4] = {false};

    // Main loop
    if (glfwWindowShouldClose(window))
    {
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Cleanup GLFW
        glfwDestroyWindow(window);
        glfwTerminate();
        running = false;
        return;
    }

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Get window size for fullscreen ImGui
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    // Create fullscreen ImGui window (no borders, no title bar)
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
    ImGui::Begin("MainWindow", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoBackground);

    // --- small circular top buttons (3) ---
    {
        // size of each circular button and spacing
        const float btnSize = 24.0f;
        const float spacing = 8.0f;

        // center the trio across the window
        float totalWidth = (3.0f * btnSize) + (2.0f * spacing);
        float startX = ((float)display_w - totalWidth) / 2.0f;
        ImGui::SetCursorPosX(startX);
        ImGui::SetCursorPosY(10.0f);

        // make buttons round by increasing FrameRounding temporarily
        ImGuiStyle &style_top = ImGui::GetStyle();
        const float oldRounding = style_top.FrameRounding;
        style_top.FrameRounding = btnSize * 0.5f; // half size -> circle

        // gray colors for button states
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.58f, 0.58f, 0.58f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.42f, 0.42f, 0.42f, 1.0f));

        for (int i = 0; i < 3; ++i)
        {
            ImGui::PushID(1000 + i); // unique ID for each top button

            if (i > 0)
                ImGui::SameLine(0.0f, spacing);

            // invisible label so nothing is drawn on the button
            char label[16];
            sprintf(label, "##topbtn%d", i);

            ImGui::Button(label, ImVec2(btnSize, btnSize));
            bool topActive = ImGui::IsItemActive();

            switch (i)
            {
            case 0:
                previousButtonState = topActive;
                break;
            case 1:
                nextButtonState = topActive;
                break;
            case 2:
                difficultyButtonState = topActive;
                break;
            }

            ImGui::PopID();
        }

        ImGui::PopStyleColor(3);
        style_top.FrameRounding = oldRounding; // restore rounding
    }

    // Calculate button size to make them square and fit nicely
    float available_width = (float)display_w - 60.0f;  // Leave some margin
    float available_height = (float)display_h - 60.0f; // Leave some margin (no controls at bottom)

    float button_size = std::min(available_width / 4.0f - 30.0f, available_height / 4.0f - 30.0f); // Square buttons with spacing
    button_size *= 0.95f;                                                                          // Make buttons 5% smaller

    // Center the button grid
    float grid_width = (button_size * 4) + (3 * 30);  // 4 buttons + 3 spaces (30px each)
    float grid_height = (button_size * 4) + (3 * 30); // 4 buttons + 3 vertical spaces (30px each)
    float start_x = ((float)display_w - grid_width) / 2.0f;
    float start_y = ((float)display_h - grid_height) / 2.0f;

    // Create 4x4 button grid
    for (int row = 0; row < 4; row++)
    {
        ImGui::SetCursorPosX(start_x);                                 // Reset X position for each row
        ImGui::SetCursorPosY(start_y + (row * (button_size + 30.0f))); // Set Y position with vertical spacing

        for (int col = 0; col < 4; col++)
        {
            int buttonIndex = (row * 4) + col;

            // Create unique button ID
            ImGui::PushID(row * 4 + col);

            if (LED::leds[buttonIndex].isLedOn)
            {
                float r = ((float)LED::leds[buttonIndex].color.r / 255);
                float g = ((float)LED::leds[buttonIndex].color.g / 255);
                float b = ((float)LED::leds[buttonIndex].color.b / 255);
                double a = LED::leds[buttonIndex].intensity;
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(r, g, b, a));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(r, g, b, a));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(r, g, b, a));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(.73333f, .73333f, .73333f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.73333f, .73333f, .73333f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.73333f, .73333f, .73333f, 1.0f));
            }

            // Square button with calculated size
            ImGui::Button("##", ImVec2(button_size, button_size));

            // Check if this specific button is being held down
            if (ImGui::IsItemActive())
            {
                button_states[row][col] = true;
                buttonData |= 1 << buttonIndex;
                // std::cout << "Button: " << buttonData << std::endl;
            }
            else
            {
                button_states[row][col] = false;
                buttonData &= ~(1 << buttonIndex);
            }

            // Pop the style colors
            ImGui::PopStyleColor(3);
            ImGui::PopID();

            // Add spacing between buttons, but not after the last column
            if (col < 3)
            {
                ImGui::SameLine(0, 30.0f); // 30px spacing between buttons (tripled from 10px)
            }
        }
    }

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.117f, 0.117f, 0.117f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void DesktopUtility::processAudioCommand(DFPlayerCommand command, void (*callback)(void))
{
    if (audioInitResult == MA_SUCCESS && command != DFPlayerCommand::Mute && command != DFPlayerCommand::Unmute && command != DFPlayerCommand::None)
    {
        audioCompleteCallback = callback;

        char filepath[64];
        sprintf(filepath, "src/assets/audio/%d.mp3", (int)command);
        ma_sound_uninit(&audioSound);
        ma_sound_init_from_file(&audioEngine, filepath, MA_SOUND_FLAG_DECODE, NULL, NULL, &audioSound);
        ma_sound_set_end_callback(&audioSound, onMiniaudioComplete, NULL);
        ma_sound_start(&audioSound);
    }
}

void DesktopUtility::setBeepNote(MusicNote note)
{
    queueMusicNote = note;
}

void DesktopUtility::enableBeep(bool enable)
{
    if (enable)
    {
        float noteFrequency = (float)(48000000 / ((static_cast<int>(queueMusicNote) + 1) * 256));
        setTone(noteFrequency);
    }
    else
    {
        setTone(0.0f);
    }
}

uint16_t DesktopUtility::getRNGSeedValue()
{
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 255);
    return static_cast<uint8_t>(dist(rng));
}

void DesktopUtility::configureFixedUpdateTimer()
{
    std::thread workerThread(fixedUpdateThreadFunction);
    workerThread.detach();
}

void DesktopUtility::configureButtonPins()
{
}

void DesktopUtility::configureLeds()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    // Create square window (630x630, non-resizable)
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Make window non-resizable
    window = glfwCreateWindow(630, 630, "4x4 Button Grid", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        // normally returns -1 here
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Set rounded corners for buttons and adjust font size
    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 12.0f; // Rounded corners for buttons and other frames

    // Load a bigger font (simplified approach)
    ImGuiIO &io_ref = ImGui::GetIO();
    ImFontConfig font_config;
    font_config.SizePixels = 19.0f; // Bigger font (default is usually 13px)

    // Just use default font but bigger - more reliable
    io_ref.Fonts->AddFontDefault(&font_config);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void DesktopUtility::configureAudio()
{
    // Configure audio engine for voice over playback
    audioInitResult = ma_engine_init(NULL, &audioEngine);

    // Configure mini audio for beep sound playback
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate = 48000;
    config.dataCallback = audioCallback;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
    {
        printf("Failed to init audio device.\n");
    }

    ma_device_start(&device);
}

void DesktopUtility::configureRNG()
{
}
