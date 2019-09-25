#include "common/command/command_gpio.h"

CommandGpio::CommandGpio( const char *mutator, const char *accessor )
    : CommandTemplate< AM335X::Gpio >( mutator, accessor, PARAM_PIN )
    , mPin( 0 )
    , mBank( 0 )
{
    mRequiredMap[ PARAM_BANK ] = PARAMETER_CALLBACK( &CommandGpio::setBank );

    mAccessorMap[ PARAM_BANK ] = PARAMETER_CALLBACK( &CommandGpio::getBank );
    mAccessorMap[ PARAM_OUTPUT ] = PARAMETER_CALLBACK( &CommandGpio::getBankOutput );
    mAccessorMap[ PARAM_INPUT ] = PARAMETER_CALLBACK( &CommandGpio::getBankInput );

    mOptional = PARAMETER_CALLBACK( &CommandGpio::setPin );

    mOptionalAccessorMap[ PARAM_BANK ] = PARAMETER_CALLBACK( &CommandGpio::getBank );
    mOptionalAccessorMap[ PARAM_PIN ] = PARAMETER_CALLBACK( &CommandGpio::getPin );
    mOptionalAccessorMap[ PARAM_OUTPUT ] = PARAMETER_CALLBACK( &CommandGpio::getOutput );
    mOptionalAccessorMap[ PARAM_INPUT ] = PARAMETER_CALLBACK( &CommandGpio::getInput );
    mOptionalAccessorMap[ PARAM_DIR ] = PARAMETER_CALLBACK( &CommandGpio::getDirection );

    mOptionalMutatorMap[ PARAM_OUTPUT ] = PARAMETER_CALLBACK( &CommandGpio::setOutput );
    mOptionalMutatorMap[ PARAM_DIR ] = PARAMETER_CALLBACK( &CommandGpio::setDirection );
}

uint32_t CommandGpio::setBank( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsNumber( val ) ) {
        mBank = static_cast< uint32_t >( val->valueint );
        mControlObject = AM335X::Gpio::getControlObject( mBank );
        if( mControlObject == nullptr ) {
            r = Error::Code::PARAM_OUT_OF_RANGE;
        }
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandGpio::setPin( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsNumber( val ) ) {
        mPin = static_cast< uint32_t >( val->valueint );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandGpio::setOutput( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsTrue( val ) ) {
        mControlObject->setOutput( mPin, true );
    } else if( cJSON_IsFalse( val ) ) {
        mControlObject->setOutput( mPin, false );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandGpio::setDirection( cJSON *val )
{
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsString( val ) ) {
        mControlObject->setDirection( mPin, val->valuestring );
    } else {
        r = Error::Code::SYNTAX;
    }
    return r;
}

uint32_t CommandGpio::getBank( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_BANK, mControlObject->getId() );
    return r;
}

uint32_t CommandGpio::getPin( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_PIN, mPin );
    return r;
}

uint32_t CommandGpio::getOutput( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_OUTPUT, mControlObject->getOutput( mPin ) );
    return r;
}

uint32_t CommandGpio::getInput( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddBoolToObject( response, PARAM_INPUT, mControlObject->getInput( mPin ) );
    return r;
}

uint32_t CommandGpio::getDirection( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddStringToObject( response, PARAM_DIR, mControlObject->getDirection( mPin ) );
    return r;
}

uint32_t CommandGpio::getBankOutput( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_OUTPUT, mControlObject->getOutput() );
    return r;
}

uint32_t CommandGpio::getBankInput( cJSON *response )
{
    uint32_t r = Error::Code::NONE;
    cJSON_AddNumberToObject( response, PARAM_INPUT, mControlObject->getInput() );
    return r;
}
