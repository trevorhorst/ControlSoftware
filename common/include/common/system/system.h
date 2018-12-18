#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>

#include "common/singleton.h"
#include "common/control/control.h"

#define SYSTEM_HOSTNAME_SIZE_MAX 128

class System
        : public Singleton< System >
        , public Control
{
    friend class Singleton< System >;
public:
    System();
    void getHostname( char *hostname );
};

#endif // SYSTEM_H
