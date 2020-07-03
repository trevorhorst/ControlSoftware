#include "http/command.h"

CommandServer::CommandServer()
    : CommandTemplate< Http::Server >( COMMAND_SERVER, COMMAND_QSERVER )
{
    mAccessorMap[ PARAM_ACTIVE ] = PARAMETER_CALLBACK( &CommandServer::getActive );
    mAccessorMap[ PARAM_PORT ]   = PARAMETER_CALLBACK( &CommandServer::getPort );
    mAccessorMap[ PARAM_SECURE ] = PARAMETER_CALLBACK( &CommandServer::getSecure );
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

uint32_t CommandServer::getSecure( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_SECURE, mControlObject->isSecure() );
    return r;
}
