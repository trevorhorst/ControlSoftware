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

/**
 * @brief Command call wrapper
 * @param params Parameters to pass to the command
 * @return Response to the call
 */
cJSON *CommandContainer::call( cJSON *params )
{
    cJSON *rsp = nullptr;

    switch( mType ) {
    case Command::ACCESSOR:
        rsp = mCmdObj->access( params );
        break;
    case Command::MUTATOR:
        rsp = mCmdObj->mutate( params );
        break;
    }

    return rsp;
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

/**
 * @brief Handles a command
 * @param cmdStr Command string to handle
 * @return Response to the call
 */
cJSON *CommandHandler::handle( const char *cmdStr )
{
    cJSON *rsp = nullptr;

    bool ok = true;
    cJSON *parsed = cJSON_Parse( cmdStr );
    if( cJSON_IsInvalid( parsed ) || cJSON_IsNull( parsed ) ) {
        /// @todo The cmdStr is invalid, handle this
        printf( "%s: Command string is invalid\n", __FUNCTION__ );
        ok = false;
    }

    cJSON *cmd = cJSON_GetObjectItem( parsed, PARAM_COMMAND );
    if( ok && cJSON_IsString( cmd ) ) {
        auto it = mCommandMap.find( cmd->valuestring );
        if( it == mCommandMap.end() ) {
            // The command does not exist
            printf( "%s: The '%s' command does not exist\n"
                    , __FUNCTION__, cmd->valuestring );
            ok = false;
        } else {
            cJSON *params = cJSON_GetObjectItem( parsed, PARAM_PARAMS );
            rsp = it->second->call( params );
        }
    } else {
        /// @todo The cmd is not the type we expect, handle this
        ok = false;
    }

    cJSON_Delete( parsed );
    return rsp;
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
