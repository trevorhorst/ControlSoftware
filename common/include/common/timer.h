#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <stdint.h>
#include <thread>

#include "common/control/control_template.h"
#include "common/logger/log.h"

class Timer
        : public ControlTemplate< Timer >
{
public:

    enum Type {
        SINGLE_SHOT = 0
        , INTERVAL  = 1
    };

    Timer( int32_t delayMs, Type type, std::function< void() > callback );
    ~Timer();

    void start();
    void stop();

    bool isEnabled();
    uint32_t setEnable( bool enable );
private:
    bool mEnable;
    int32_t mDelay;
    Type mType;

    std::thread *mThread;
    std::function< void() > mCallback;
};

#endif // TIMER_H
