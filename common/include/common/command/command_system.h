#ifndef COMMAND_SYSTEM_H
#define COMMAND_SYSTEM_H

#include "common/command/command.h"
#include "common/system/system.h"

#define COMMAND_SYSTEM  "system"
#define COMMAND_QSYSTEM "qsystem"

class CommandSystem
        : public Command
{
public:
    CommandSystem();
};

#endif // COMMAND_SYSTEM_H
