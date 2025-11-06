#include "desktoputility.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <led.h>
#include <sstream>
#include <thread>

DesktopUtility::DesktopUtility()
    : DeviceUtility(),
      window()
{
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
    return false;
}

bool DesktopUtility::getNextButtonState()
{
    return false;
}

bool DesktopUtility::getDifficultyButtonState()
{
    return false;
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
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

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

            // Create button with position label (no numbers)
            char button_label[32];
            sprintf(button_label, "(%d,%d)", row, col);

            // Square button with calculated size
            ImGui::Button(button_label, ImVec2(button_size, button_size));

            // Check if this specific button is being held down
            if (ImGui::IsItemActive())
            {
                button_states[row][col] = true;
                buttonData |= 1 << buttonIndex;
                std::cout << "Button: " << buttonData << std::endl;
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
    glClearColor(0.117f, 0.117f, 0.117f, 1.00f); // VS Code dark theme background (#1E1E1E)
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void DesktopUtility::processAudioCommand(DFPlayerCommand command, void (*callback)(void))
{
}

void DesktopUtility::setBeepNote(MusicNote note)
{
}

void DesktopUtility::enableBeep(bool enable)
{
}

uint16_t DesktopUtility::getRNGSeedValue()
{
    return 0;
}

// Atomic flag to control the thread
std::atomic<bool> running{true};

// Function that will run in the thread
void threadFunction()
{
    while (running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
        if (!DeviceUtility::fixedUpdate)
            DeviceUtility::fixedUpdate = true;
    }
}

void DesktopUtility::configureFixedUpdateTimer()
{
    std::thread workerThread(threadFunction);
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
}

void DesktopUtility::configureRNG()
{
}
