#include "common/drivers/led.h"

/**
 * @brief Constructor
 */
Led::Led()
{
}

uint32_t Led::setEnable( bool enable )
{
    uint32_t r = Error::Code::NONE;
    mEnable = enable;
    return r;
}

bool Led::isEnabled()
{
    return mEnable;
}
