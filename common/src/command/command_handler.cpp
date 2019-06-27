#include "common/command/command_handler.h"

/**
 * @brief Constructor
 * @param type Command type
 * @param cmdObj Command object
 */
CommandContainer::CommandContainer( Command::Type type, Command *cmdObj )
    : mType( type )
    , mCmdObj( cmdObj )
{

}

const char *CommandContainer::getUsage()
{
    return mCmdObj->usage();
}

/**
 * @brief Command call wrapper
 * @param params Parameters to pass to the command
 * @return Response to the call
 */
bool CommandContainer::call( cJSON *params, cJSON *response )
{
    return mCmdObj->call( params, response, mType );
}

/**
 * @brief CommandHandler Constructor
 */
CommandHandler::CommandHandler()
    : mCommandMap{}
{

}

/**
 * @brief CommandHandler Destructor
 */
CommandHandler::~CommandHandler()
{
    for( auto it = mCommandMap.begin(); it != mCommandMap.end(); it++ ) {
        if( it->second ) {
            delete it->second;
        }
    }
}

CommandMap *CommandHandler::getCommandMap()
{
    return &mCommandMap;
}

/**
 * @brief CommandHandler::getCommandUsage
 * @param cmd
 * @return
 */
const char *CommandHandler::getCommandUsage( const char *cmd )
{
    const char *usage = nullptr;
    auto it = mCommandMap.find( cmd );

    if( it == mCommandMap.end() ) {
    } else {
        usage = it->second->getUsage();
    }

    return usage;
}

/**
 * @brief Handles a command
 * @param cmdStr Command string to handle
 * @return Response to the call
 */
bool CommandHandler::handle( const char *cmdStr, cJSON *response )
{
    bool ok = true;

    cJSON *parsed = cJSON_Parse( cmdStr );
    if( parsed == nullptr ) {
        /// @todo The cmdStr is invalid, handle this response
        printf( "%s: Command string is invalid\n", __FUNCTION__ );
        ok = false;
    }

    cJSON *cmd = cJSON_GetObjectItem( parsed, PARAM_COMMAND );
    if( ok && cJSON_IsString( cmd ) ) {

        auto it = mCommandMap.find( cmd->valuestring );
        if( it == mCommandMap.end() ) {
            /// @todo The command does not exist, handle this response
            printf( "%s: The '%s' command does not exist\n"
                    , __FUNCTION__, cmd->valuestring );
            ok = false;
        } else {
            // The command exists
            cJSON *params = cJSON_GetObjectItem( parsed, PARAM_PARAMS );
            bool success = it->second->call( params, response );

            // Add the success parameter to the response object
            cJSON_AddBoolToObject( response, PARAM_SUCCESS, success );
        }

    } else {
        /// @todo The cmd is not the type we expect, handle this response
        ok = false;
    }

    cJSON_Delete( parsed );
    return ok;
}

/**
 * @brief Adds a command to the command map
 * @param cmd Command to add to the map
 */
void CommandHandler::addCommand( Command *cmd )
{
    if( cmd->isAccessible() ) {
        mCommandMap[ cmd->getAccessorName() ]
                = new CommandContainer( Command::Type::ACCESSOR, cmd );
    }

    if( cmd->isMutable() ) {
        mCommandMap[ cmd->getMutatorName() ]
                = new CommandContainer( Command::Type::MUTATOR, cmd );
    }
}
