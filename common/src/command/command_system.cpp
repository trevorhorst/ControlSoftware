#include "common/command/command_system.h"

CommandSystem::CommandSystem()
    : CommandTemplate< System >( COMMAND_SYSTEM, COMMAND_QSYSTEM )
{
    mMutatorMap[ PARAM_HOSTNAME ]
            = PARAMETER_CALLBACK( &CommandSystem::setHostname );

    mAccessorMap[ PARAM_HOSTNAME ]
            = PARAMETER_CALLBACK( &CommandSystem::getHostname );
    mAccessorMap[ PARAM_INTERFACE ]
            = PARAMETER_CALLBACK( &CommandSystem::getInterfaces );
}

uint32_t CommandSystem::getHostname( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_HOSTNAME, mControlObject->getHostname() );
    return r;
}

uint32_t CommandSystem::getInterfaces( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    (void)response;
    // mControlObject->getInterfaces();
    return r;
}

uint32_t CommandSystem::setHostname( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        r = mControlObject->setHostname( val->string );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}
