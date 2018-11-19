#ifndef HARDWARE_BEAGLEBONEBLACK_H
#define HARDWARE_BEAGLEBONEBLACK_H

// #include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "beagleboneblack/resources/resources.h"

#include "common/singleton.h"
#include "common/hardware/hardware.h"

class BeagleboneBlack
        : public Hardware
        , public Singleton< BeagleboneBlack >
{
    friend class Singleton< BeagleboneBlack >;
public:
private:
    BeagleboneBlack();
    ~BeagleboneBlack();

    const char *mIndexPage;
};

#endif // HARDWARE_BEAGLEBONEBLACK_H
