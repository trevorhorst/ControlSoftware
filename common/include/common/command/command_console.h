#ifndef COMMAND_CONSOLE_H
#define COMMAND_CONSOLE_H

#include "common/command/command_template.h"
#include "common/console/console.h"

#define COMMAND_CONSOLE     "console"
#define COMMAND_QCONSOLE    "qconsole"

class CommandConsole
        : public CommandTemplate< Console >
{
public:
    CommandConsole();

    virtual uint32_t setQuit( cJSON *val );
};

#endif // COMMAND_CONSOLE_H
