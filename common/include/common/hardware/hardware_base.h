/**
 * @file hardware.h
 * @author Trevor Horst
 * @brief Base class for the hardware library
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "common/command/command_handler.h"
#include "common/transport/client.h"

class HardwareBase
{
public:

    HardwareBase();

    virtual ~HardwareBase();

    virtual CommandHandler *getCommandHandler();
    virtual void addCommand( Command* cmd );

    virtual Transport::Client *getClient();

    static void setSimulated( bool simulated );
    static bool isSimulated();

protected:

private:
    CommandHandler mCommandHandler;
    static bool mSimulated;
};

#endif // HARDWARE_H
