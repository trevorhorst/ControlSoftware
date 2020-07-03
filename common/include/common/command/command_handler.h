#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "common/common_types.h"
#include "common/control/control_template.h"
#include "common/command/command.h"


/**
 * @brief Contains information regarding the required call
 */
class CommandContainer
{
public:
    CommandContainer( Command::Type type, Command *cmdObj );
    const char *getUsage();
    bool call( cJSON *params, cJSON *response );
private:
    Command::Type mType;
    Command *mCmdObj;
};

using CommandMap = Types::CharHashMap< CommandContainer* >;

/**
 * @brief Handles any received commands by calling the required methods
 */
class CommandHandler
        : public ControlTemplate< CommandHandler >
{
public:

    CommandHandler();
    ~CommandHandler();

    bool handle( const char *cmdStr, cJSON *response );
    void addCommand( Command *cmd );
    const char *getCommandUsage( const char *cmd );
    CommandMap *getCommandMap();
private:
    CommandMap mCommandMap;
};

#endif // COMMAND_HANDLER_H
