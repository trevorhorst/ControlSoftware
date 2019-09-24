#include "common/system/system.h"

/**
 * @brief Constructor
 */
System::System()
{
}

const char *System::getHostname()
{
    gethostname( mHostname, SYSTEM_HOSTNAME_SIZE_MAX );
    return mHostname;
}
