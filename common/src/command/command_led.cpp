#include "common/command/command_led.h"

CommandLed::CommandLed()
    : CommandTemplate< Led >( COMMAND_LED, COMMAND_QLED )
{
    mRequiredMap[ PARAM_ID ] = PARAMETER_CALLBACK( &CommandLed::setId );

    mMutatorMap[ PARAM_ENABLE ] = PARAMETER_CALLBACK( &CommandLed::setEnable );

    mAccessorMap[ PARAM_ENABLE ] = PARAMETER_CALLBACK( &CommandLed::getEnable );
}

uint32_t CommandLed::setId(cJSON *val)
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsNumber( val ) ) {
        mControlObject = Led::getControlObject( static_cast< uint32_t >( val->valuedouble ) );
        if( mControlObject == nullptr ) {
            r = Error::Code::PARAM_OUT_OF_RANGE;
        }
    } else {
        // The type isn't what we expect, syntax error
        r = Error::Code::SYNTAX;
    }

    return r;
}

uint32_t CommandLed::setEnable( cJSON *val )
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

uint32_t CommandLed::getEnable(cJSON *response)
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_ENABLE, mControlObject->isEnabled() );
    return r;
}
