#ifndef COMMAND_CONSOLE_H
#define COMMAND_CONSOLE_H

#include "common/command/command.h"
#include "common/console/console.h"

#define COMMAND_CONSOLE "console"

class CommandConsole : public Command
{
public:
    CommandConsole();
    virtual uint32_t setQuit( cJSON *val );
};

#endif // COMMAND_CONSOLE_H
