/**
 * @file hardware.h
 * @author Trevor Horst
 * @brief Base class for the hardware library
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "common/command/command_handler.h"

class HardwareBase
{
public:

    HardwareBase();

    virtual ~HardwareBase();

    virtual CommandHandler *getCommandHandler();
    virtual void addCommand( Command* cmd );

    static void setSimulated( bool simulated );
    static bool isSimulated();

private:
    CommandHandler mCommandHandler;
    static bool mSimulated;
};

#endif // HARDWARE_H
