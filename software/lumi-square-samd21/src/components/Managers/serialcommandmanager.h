#ifndef SERIALCOMMANDMANAGER_H
#define SERIALCOMMANDMANAGER_H

#include <stdint.h>

class SerialCommandManager
{
public:
    static SerialCommandManager &Instance(void);
    static void onFixedUpdate();
    void enableUART(bool enable);
    void usartTransmit(uint8_t data);

private:
    SerialCommandManager();
    SerialCommandManager(const SerialCommandManager &) = delete;
    void operator=(const SerialCommandManager &) = delete;
};

#endif