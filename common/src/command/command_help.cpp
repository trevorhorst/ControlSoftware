#include "common/command/command_help.h"

CommandHelp::CommandHelp()
    : CommandTemplate< CommandHandler > ( COMMAND_EMPTY, COMMAND_HELP )
{
    mAccessorMap[ PARAM_COMMANDS ] = PARAMETER_CALLBACK( &CommandHelp::getCommandMap );
};

uint32_t CommandHelp::getCommandMap( cJSON *response )
{
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