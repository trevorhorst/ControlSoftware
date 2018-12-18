#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "common/common_types.h"
#include "common/command/command.h"


/**
 * @brief Contains information regarding the required call
 */
class CommandContainer
{
public:
    CommandContainer( Command::Type type, Command *cmdObj );
    cJSON *call( cJSON *params );
private:
    Command::Type mType;
    Command *mCmdObj;
};

using CommandMap = CharHashMap< CommandContainer* >;

/**
 * @brief Handles any received commands by calling the required methods
 */
class CommandHandler
{
public:

    CommandHandler();
    ~CommandHandler();

    cJSON *handle( const char *cmdStr );
    void addCommand( Command *cmd );
private:
    CommandMap mCommandMap;
};

#endif // COMMAND_HANDLER_H
