/**
 * @file hardware.h
 * @author Trevor Horst
 * @brief Base class for the hardware library
 */

#ifndef HARDWARE_H
#define HARDWARE_H

#include "common/command/command_handler.h"

class Hardware
{
public:

    /**
     * @brief Constructor
     */
    Hardware()
    {}

    /**
     * @brief Destructor
     */
    virtual ~Hardware()
    {}

    /**
     * @brief Retrieves a pointer to the command handler
     * @return Pointer to the command handler
     */
    virtual CommandHandler *getCommandHandler()
    {
        return &mCommandHandler;
    }

    /**
     * @brief Adds a command to the local command handler
     * @param cmd Desired command to add
     */
    virtual void addCommand( Command* cmd )
    {
        mCommandHandler.addCommand( cmd );
    }

protected:

private:
    CommandHandler mCommandHandler;
};

#endif // HARDWARE_H
