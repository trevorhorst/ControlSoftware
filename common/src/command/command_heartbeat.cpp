#include "common/command/command_heartbeat.h"

CommandHeartbeat::CommandHeartbeat()
    : CommandTemplate< Timer >( COMMAND_HEARTBEAT, COMMAND_QHEARTBEAT )
{
    mMutatorMap[ PARAM_ENABLE ] = PARAMETER_CALLBACK( &CommandHeartbeat::setEnable );

    mAccessorMap[ PARAM_ENABLE ] = PARAMETER_CALLBACK( &CommandHeartbeat::getEnable );
}

uint32_t CommandHeartbeat::setEnable(cJSON *val)
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsTrue( val ) ) {
        mControlObject->setEnable( true );
    } else if( cJSON_IsFalse( val ) ) {
        mControlObject->setEnable( false );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandHeartbeat::getEnable(cJSON *response)
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_ENABLE, mControlObject->isEnabled() );
    return r;
}
