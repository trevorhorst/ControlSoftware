#include "common/command/command_help.h"

CommandHelp::CommandHelp()
    : CommandTemplate< CommandHandler > ( COMMAND_EMPTY, COMMAND_HELP )
{
    mRequiredMap[ PARAM_COMMAND_ ] = PARAMETER_CALLBACK( &CommandHelp::setCommand );

    // mAccessorMap[ PARAM_COMMANDS ] = PARAMETER_CALLBACK( &CommandHelp::getCommandMap );
    mAccessorMap[ PARAM_COMMANDS ] = PARAMETER_CALLBACK( &CommandHelp::getCommandUsage );
}

uint32_t CommandHelp::getCommandMap( cJSON *response )
{
    /// @todo There is a memory leak in this method

    uint32_t r = Error::Code::NONE;
    // Create an array object
    cJSON *array = cJSON_CreateArray();
    array = cJSON_AddArrayToObject( response, PARAM_COMMANDS );
    // Iterate through the map for the available commands
    CommandMap *map = mControlObject->getCommandMap();
    for( CommandMap::const_iterator it = map->begin(); it != map->end(); it++ ) {
        cJSON *str = cJSON_CreateString( it->first );
        cJSON_AddItemToArray( array, str );
    }
    return r;
}

uint32_t CommandHelp::setCommand( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        CommandMap *map = mControlObject->getCommandMap();
        auto it = map->find( val->valuestring );
        if( it == map->end() ) {
            r = Error::Code::PARAM_INVALID;
        } else {
            mUsage = it->second->getUsage();
        }
    }
    return r;
}

uint32_t CommandHelp::getCommandUsage( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_COMMAND, mUsage );
    return r;
}
