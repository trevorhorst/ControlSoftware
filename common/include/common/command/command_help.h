#ifndef COMMAND_HELP_H
#define COMMAND_HELP_H

#include "common/command/command_template.h"
#include "common/command/command_handler.h"

#define COMMAND_HELP    "help"

#define PARAM_COMMANDS  "commands"

class CommandHelp
        : public CommandTemplate< CommandHandler >
{
public:
    CommandHelp();
    uint32_t getCommandMap( cJSON *response );
    uint32_t getCommandUsage( cJSON *response );
};

#endif // COMMAND_HELP_H
