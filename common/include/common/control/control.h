#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <stdint.h>

#include "common/error/error.h"
#include "common/logger/log.h"

class Control
{
public:
    Control();
    virtual ~Control();

    uint32_t setVerbose( bool verbose );
    bool isVerbose();

private:
    bool mVerbose;
};

#endif // CONTROL_H
