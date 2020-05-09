#include "common/hardware/hardware_base.h"

bool HardwareBase::mSimulated = false;

bool HardwareBase::isSimulated()
{
    return mSimulated;
}

void HardwareBase::setSimulated( bool simulated )
{
    mSimulated = simulated;
}

/**
 * @brief Constructor
 */
HardwareBase::HardwareBase()
    : mClient( nullptr )
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

Transport::Client *HardwareBase::getClient()
{
    return mClient;
}
