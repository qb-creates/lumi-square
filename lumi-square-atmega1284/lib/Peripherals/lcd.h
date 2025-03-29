#ifndef LCD_H
#define LCD_H

#include "./oledbuffer.h"

class LCD
{
public:
    LCD(const LCD &) = delete;
    void operator=(const LCD &) = delete;
    static LCD &Instance();
    void initializeDisplay(void);
    void clearDisplay();
    void clearTopBuffer();
    void clearBottomBuffer();
    void writeToDifficultyBuffer(const uint8_t imageData[33]);
    void writeToSpeakerBuffer(const uint8_t imageData[27]);
    void writeToTopBuffer(const char *data);
    void writeToBottomBuffer(const char *data);
    void writeToBottomLeftBuffer(uint8_t value);
    void writeToBottomRightBuffer(uint8_t value);

private:
    LCD();
    const uint8_t OLED_SLAVE_ADDRESS;
    const uint8_t COMMAND_CONTROL_BYTE_CONTINUATION;
    const uint8_t LAST_COMMAND_CONTROL_BYTE;
    const uint8_t WRITE_CONTROL_BYTE_CONTINUATION;
    const uint8_t LAST_WRITE_CONTROL_BYTE;
    const OLEDBuffer difficultyBuffer;
    const OLEDBuffer speakerBuffer;
    const OLEDBuffer topBuffer;
    const OLEDBuffer bottomBuffer;
    uint8_t bottomLeftBufferLength;
    uint8_t bottomRightBufferLength;
    void writeStringToBuffer(OLEDBuffer buffer, const char *data);
    void writeNumberToBuffer(OLEDBuffer buffer, uint8_t value, uint8_t *test);
    void writeToBuffer(OLEDBuffer buffer, const uint8_t *imageData, uint8_t columnOffset);
    void oledSendCommand(uint8_t command, uint8_t controlByte);
};

#endif