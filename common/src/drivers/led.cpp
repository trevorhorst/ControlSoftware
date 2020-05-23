#include "common/drivers/led.h"

/**
 * @brief Constructor
 */
Led::Led(AM335X::Gpio *bank, uint32_t pin)
    : mEnable( false )
    , mPin( pin )
    , mBank( bank )
{
}

uint32_t Led::setEnable( bool enable )
{
    uint32_t error = Error::Code::NONE;

    error = mBank->setDirection( mPin, AM335X::Gpio::str_output );
    if( error == Error::Code::NONE ) {
        mBank->setOutput( mPin, enable );
    }

    if( error == Error::Code::NONE ) {
        mEnable = enable;
    }

    return error;
}

bool Led::isEnabled()
{
    return mEnable;
}
