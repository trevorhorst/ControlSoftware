#include "common/hardware/hardware_base.h"

/**
 * @brief Constructor
 */
HardwareBase::HardwareBase()
{

}

/**
 * @brief Destructor
 */
HardwareBase::~HardwareBase()
{

}

/**
 * @brief Retrieves a pointer to the command handler
 * @return Pointer to the command handler
 */
CommandHandler *HardwareBase::getCommandHandler()
{
    return &mCommandHandler;
}

/**
 * @brief Adds a command to the local command handler
 * @param cmd Desired command to add
 */
void HardwareBase::addCommand( Command *cmd )
{
    mCommandHandler.addCommand( cmd );
}
