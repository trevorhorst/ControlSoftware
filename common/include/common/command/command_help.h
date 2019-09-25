#ifndef COMMAND_HELP_H
#define COMMAND_HELP_H

#include "common/command/command_template.h"
#include "common/command/command_handler.h"

#define COMMAND_HELP    "help"

#define PARAM_COMMAND_  "command"
#define PARAM_COMMANDS  "commands"

#define PARAM_USAGE "usage"

class CommandHelp
        : public CommandTemplate< CommandHandler >
{
public:
    CommandHelp();

    uint32_t setCommand( cJSON *response );

    uint32_t getCommandMap( cJSON *response );
    uint32_t getCommandUsage( cJSON *response );
protected:

    const char *mUsage;
};

#endif // COMMAND_HELP_H
