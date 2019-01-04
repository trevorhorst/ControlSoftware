#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>

#include "common/control/control_template.h"

#define SYSTEM_HOSTNAME_SIZE_MAX 128

class System
        : public ControlTemplate< System >
{
public:
    System();
    const char *getHostname();
private:
    char mHostname[ SYSTEM_HOSTNAME_SIZE_MAX ];
};

#endif // SYSTEM_H
