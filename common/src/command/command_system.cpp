#include "common/command/command_system.h"

CommandSystem::CommandSystem()
    : CommandTemplate< System >( COMMAND_SYSTEM, COMMAND_QSYSTEM )
{
    mAccessorMap[ PARAM_HOSTNAME ]
            = PARAMETER_CALLBACK( &CommandSystem::getHostname );

    // mControlObject = System::getControlObject( 0 );
}

uint32_t CommandSystem::getHostname( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_HOSTNAME, mControlObject->getHostname() );
    return r;
}
