#include "common/system/system.h"

System::System()
{
}

void System::getHostname( char *hostname )
{
    gethostname( hostname, SYSTEM_HOSTNAME_SIZE_MAX );
}
