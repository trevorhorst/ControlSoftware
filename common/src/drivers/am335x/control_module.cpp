#include "common/drivers/am335x/control_module.h"

namespace AM335X {

const uint32_t addr_control_module = 0x44E10000;

/**
 * @brief Constructor
 */
ControlModule::ControlModule( uint32_t address, bool simulated )
    : ControlTemplate< ControlModule >()
    , mRegister( address, simulated )
{
}

/**
 * @brief Destructor
 */
ControlModule::~ControlModule()
{
}

/**
 * @brief Dumps data read from the revision register
 */
void ControlModule::dumpRevision()
{
    printf( "CONTROL_MODULE REVISION REGISTER:\n" );
    printf( "   MAJOR: %08X\n", mRegister.map()->revision.mMajor() );
    printf( "   MINOR: %08X\n", mRegister.map()->revision.mMajor() );
}

/**
 * @brief Dumps formatted output of all pin data
 */
void ControlModule::dumpPins()
{
    // Address of the first pin
    volatile uint32_t *start = &mRegister.map()->conf_gpmc_ad0;
    // volatile uint32_t *start = &mMemMap->conf_gpmc_ad0;
    // Address immediately following the last pin
    volatile uint32_t *end   = &mRegister.map()->conf_usb1_drvvbus + 1;

    for( volatile uint32_t *pin = start; pin != end; pin++ ) {
        printf( "%3d (0x%08X): resistor( %8s | %8s ) mode( %d ) receiver( %8s ) slew( %s )\n"
                , (int) (pin - start), *pin
                , CONTROL_MODULE_CONF_MODULE_PIN_TYPESEL( *pin ) ? "pullup" : "pulldown"
                , CONTROL_MODULE_CONF_MODULE_PIN_TYPESEL( *pin ) ? "disabled" : "enabled"
                , CONTROL_MODULE_CONF_MODULE_PIN_MMODE( *pin )
                , CONTROL_MODULE_CONF_MODULE_PIN_RXACTIVE( *pin ) ? "enabled" : "disabled"
                , CONTROL_MODULE_CONF_MODULE_PIN_SLEWCTRL( *pin ) ? "slow" : "fast" );
    }
}

}
