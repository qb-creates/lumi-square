#ifndef SERIALCOMMANDMANAGER_H
#define SERIALCOMMANDMANAGER_H

#include "fixedupdate.h"
#include <stdint.h>

class SerialCommandManager : public FixedUpdateEventListener
{
public:
    static SerialCommandManager &Instance(void);
    void onFixedUpdate() override;
    void enableUART(bool enable);
    void usartTransmit(uint8_t data);

private:
    SerialCommandManager();
    SerialCommandManager(const SerialCommandManager &) = delete;
    void operator=(const SerialCommandManager &) = delete;
};

#endif