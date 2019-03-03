#include "common/hardware/hardware.h"

/**
 * @brief Constructor
 */
Hardware::Hardware()
{

}

/**
 * @brief Destructor
 */
Hardware::~Hardware()
{

}

/**
 * @brief Retrieves a pointer to the command handler
 * @return Pointer to the command handler
 */
CommandHandler *Hardware::getCommandHandler()
{
    return &mCommandHandler;
}

/**
 * @brief Adds a command to the local command handler
 * @param cmd Desired command to add
 */
void Hardware::addCommand( Command *cmd )
{
    mCommandHandler.addCommand( cmd );
}
