#ifndef DEVICEUTILITY_H
#define DEVICEUTILITY_H

#include "audiocommand.h"
#include "musicnote.h"
#include <stdint.h>

#ifndef DESKTOP_SIMULATION
#define FIXED_UPDATE_INTERVAL_MS 16
#else
#define FIXED_UPDATE_INTERVAL_MS 16
#endif

#define FIXED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - ((F_CPU / 1024UL) * FIXED_UPDATE_INTERVAL_MS) / 1000UL)

#define LED_UPDATE_INTERVAL_US 21845
#define LED_UPDATE_TIMER_PRELOAD_VALUE (65536UL - (uint32_t)((((uint64_t)(F_CPU) / 16UL) * LED_UPDATE_INTERVAL_US) / 1000000ULL))

class DeviceUtility
{

public:
    static const int8_t DELTA_TIME;
    static volatile bool fixedUpdate;

public:
    /**
     * @brief Gets the singleton instance of the DeviceUtility.
     *
     * Returns a reference to the single instance of the device utility.
     * The concrete implementation (SAMD21J18Utility or DesktopUtility)
     * is determined at compile time based on build configuration.
     *
     * @return DeviceUtility& Reference to the singleton device utility instance
     */
    static DeviceUtility &Instance();

    /**
     * @brief Configures all hardware peripherals for the target device.
     *
     * Initializes and configures all necessary hardware components including:
     * - Fixed update timer for 16ms game loop timing
     * - Button matrix and navigation buttons
     * - LED drivers and SPI communication
     * - Audio subsystem (PWM for tones, UART for voice)
     * - Random number generator seed timer
     *
     * Must be called once during system initialization before using any other functions.
     */
    virtual void configure() = 0;

    /**
     * @brief Scans the 4x4 button matrix to detect pressed buttons.
     *
     * Performs a complete scan of the button matrix by sequentially activating
     * each column and reading the row states. Returns a bitmask representing
     * which buttons are currently pressed.
     *
     * The returned value uses a positional encoding where each bit represents
     * a specific button position in the 4x4 grid.
     *
     * @return uint32_t Bitmask of pressed buttons (1 = pressed, 0 = not pressed)
     */
    virtual uint32_t scanButtonMatrix() = 0;

    /**
     * @brief Gets the current state of the previous navigation button.
     *
     * Reads the hardware state of the dedicated previous/back button.
     * This button is separate from the 4x4 matrix and used for menu navigation.
     *
     * @return true if the previous button is currently pressed
     * @return false if the previous button is not pressed
     */
    virtual bool getPreviousButtonState() = 0;

    /**
     * @brief Gets the current state of the next navigation button.
     *
     * Reads the hardware state of the dedicated next/forward button.
     * This button is separate from the 4x4 matrix and used for menu navigation.
     *
     * @return true if the next button is currently pressed
     * @return false if the next button is not pressed
     */
    virtual bool getNextButtonState() = 0;

    /**
     * @brief Gets the current state of the difficulty/mute button.
     *
     * Reads the hardware state of the dedicated difficulty selection button.
     * This button serves dual purposes: difficulty selection in menus and
     * audio mute toggle when held for extended periods.
     *
     * @return true if the difficulty button is currently pressed
     * @return false if the difficulty button is not pressed
     */
    virtual bool getDifficultyButtonState() = 0;

    /**
     * @brief Updates LED colors for one button in the matrix using multiplexed display.
     *
     * Refreshes the color data for a single LED in the 4x4 button matrix.
     * Uses time-multiplexed PWM to display 8-bit color depth across all LEDs.
     * This function should be called continuously at high frequency to maintain
     * smooth color display without flicker.
     *
     * @param ledColorData 3D array containing 8-bit PWM color data for each LED
     *                     Format: [column][row][pwm_bit_plane]
     */
    virtual void refreshButtonColor(volatile uint16_t ledColorData[4][4][8]) = 0;

    /**
     * @brief Processes audio commands for voice playback with optional completion callback.
     *
     * Sends commands to the audio subsystem for voice-over playback. For hardware
     * implementations, this communicates with a DFPlayer module via UART. For desktop
     * implementations, this triggers audio file playback.
     *
     * The optional callback function is invoked when the audio command completes,
     * allowing for sequential audio playback and state management.
     *
     * @param command The audio command/track to play from the DFPlayerCommand enum
     * @param callback Optional function pointer called when playback completes (default: nullptr)
     */
    virtual void processAudioCommand(DFPlayerCommand command, void (*callback)(void) = nullptr) = 0;

    /**
     * @brief Configures the frequency for system beep/tone generation.
     *
     * Sets up the tone generator with the specified musical note frequency.
     * This prepares the audio hardware for tone playback but does not immediately
     * start sound output. Use enableBeep() to actually start/stop the tone.
     *
     * For hardware: Configures PWM timer compare values for the piezo buzzer.
     * For desktop: Sets up audio synthesis parameters for the specified frequency.
     *
     * @param note Musical note from the MusicNote enum (e.g., MusicNote::C4)
     */
    virtual void setBeepNote(MusicNote note) = 0;

    /**
     * @brief Enables or disables system beep/tone output.
     *
     * Controls whether the tone configured by setBeepNote() is actually output.
     * This allows for precise timing control of beep start/stop without
     * reconfiguring the frequency.
     *
     * For hardware: Enables/disables the PWM timer output to the piezo buzzer.
     * For desktop: Starts/stops audio synthesis playback.
     *
     * @param enable true to start tone output, false to stop tone output
     */
    virtual void enableBeep(bool enable) = 0;

    /**
     * @brief Gets a hardware-derived random seed value.
     *
     * Retrieves a pseudo-random value from hardware sources for seeding
     * random number generators. The randomness comes from hardware timer
     * variations and other non-deterministic hardware states.
     *
     * For hardware: Reads from a free-running timer counter.
     * For desktop: Uses system-provided random sources.
     *
     * @return uint16_t Random seed value for initializing RNG algorithms
     */
    virtual uint16_t getRNGSeedValue() = 0;

private:
    /**
     * @brief Configures the fixed update timer for consistent game loop timing.
     *
     * Sets up a hardware timer to generate interrupts at 16ms intervals (62.5 Hz)
     * for consistent game logic updates. The timer interrupt sets the fixedUpdate
     * flag to true, signaling the main loop that it's time for the next update cycle.
     *
     * For hardware: Configures TC3 timer with prescaler and preload values.
     * For desktop: Sets up high-resolution timer or system timer callbacks.
     */
    virtual void configureFixedUpdateTimer() = 0;

    /**
     * @brief Configures GPIO pins for button matrix and navigation buttons.
     *
     * Initializes all button-related pins including:
     * - 4x4 button matrix column outputs and row inputs with pull-up resistors
     * - Individual navigation buttons (previous, next, difficulty) as inputs
     * - Proper pin multiplexing and electrical configuration
     *
     * For hardware: Configures PORT registers, pin directions, and pull-up resistors.
     * For desktop: Initializes input handling and key mapping.
     */
    virtual void configureButtonPins() = 0;

    /**
     * @brief Configures LED drivers and communication interface.
     *
     * Sets up the hardware interface for controlling the 4x4 LED matrix including:
     * - SPI communication for LED driver chips
     * - GPIO pins for LED column selection and data latching
     * - Timing parameters for proper LED multiplexing
     *
     * For hardware: Configures SERCOM4 SPI, PORT pins, and timing registers.
     * For desktop: Initializes graphics context and LED simulation display.
     */
    virtual void configureLeds() = 0;

    /**
     * @brief Configures audio subsystem for both voice and tone generation.
     *
     * Initializes all audio-related hardware including:
     * - PWM timer for piezo buzzer tone generation
     * - UART communication for DFPlayer voice module
     * - DMA channels for efficient audio data transfer
     * - Interrupt handlers for audio completion callbacks
     *
     * For hardware: Configures TC7 PWM, SERCOM5 UART, DMAC, and interrupts.
     * For desktop: Initializes audio playback libraries and device interfaces.
     */
    virtual void configureAudio() = 0;

    /**
     * @brief Configures random number generator seed timer.
     *
     * Sets up a free-running timer used as a source of entropy for random
     * number generation. The timer runs continuously at high speed to provide
     * non-deterministic values for seeding RNG algorithms.
     *
     * For hardware: Configures TC5 timer in continuous counting mode.
     * For desktop: Initializes system random number facilities.
     */
    virtual void configureRNG() = 0;

public:
    /**
     * @brief Protected constructor for abstract base class.
     *
     * Constructor is protected to prevent direct instantiation of the abstract
     * DeviceUtility class. Concrete implementations (SAMD21J18Utility, DesktopUtility)
     * can call this constructor during their initialization.
     */
    DeviceUtility();

private:
    /**
     * @brief Deleted copy constructor to prevent copying.
     *
     * Singleton pattern enforcement - prevents creation of additional instances
     * through copy construction. DeviceUtility instances cannot be copied.
     */
    DeviceUtility(const DeviceUtility &) = delete;

    /**
     * @brief Deleted assignment operator to prevent copying.
     *
     * Singleton pattern enforcement - prevents assignment of DeviceUtility instances.
     * This ensures only one instance exists throughout the application lifetime.
     */
    void operator=(const DeviceUtility &) = delete;
};

#endif