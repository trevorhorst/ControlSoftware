#ifndef DATETIME_H
#define DATETIME_H

#include <chrono>
#include <ctime>

#include "common/control/control_template.h"

class DateTime
        : public ControlTemplate< DateTime >
{
public:
    DateTime();
    const char *getDateTime();
};

#endif // DATETIME_H
