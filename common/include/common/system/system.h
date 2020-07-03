#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netdb.h>
#include <linux/if_link.h>

#include "common/control/control_template.h"

#define SYSTEM_HOSTNAME_SIZE_MAX 128

class System
        : public ControlTemplate< System >
{
public:
    System();
    const char *getHostname();
    uint32_t getInterfaces();

    uint32_t setHostname(const char *hostname);
private:
    char mHostname[ SYSTEM_HOSTNAME_SIZE_MAX ];
};

#endif // SYSTEM_H
