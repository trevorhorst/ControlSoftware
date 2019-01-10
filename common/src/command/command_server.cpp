#include "common/command/command_server.h"

CommandServer::CommandServer()
    : CommandTemplate< HttpServer >( COMMAND_SERVER, COMMAND_QSERVER )
{
    mAccessorMap[ PARAM_ACTIVE ] = PARAMETER_CALLBACK( &CommandServer::getActive );
    mAccessorMap[ PARAM_PORT ]   = PARAMETER_CALLBACK( &CommandServer::getPort );
}

uint32_t CommandServer::getPort( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_PORT, mControlObject->getPort() );
    return r;
}

uint32_t CommandServer::getActive( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_ACTIVE, mControlObject->isRunning() );
    return r;
}
