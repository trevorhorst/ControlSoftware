#include "common/drivers/am335x/clock_module.h"

namespace AM335X {

const uint32_t addr_clock_module = 0x44E00000;

const uint32_t ClockModule::module_mode_disabled = 0x0;
const uint32_t ClockModule::module_mode_enabled = 0x2;

/**
 * @brief Constructor
 * @param address Register address
 * @param simulated Register simulation flag
 */
ClockModule::ClockModule( uint32_t address, bool simulated )
    : ControlTemplate< ClockModule >()
    , mMap( address, simulated )
{
    // Should we be enabling the clocks here or outside when they get used?
    enableGpioClocks();

    dumpModes();
}

/**
 * @brief Destructor
 */
ClockModule::~ClockModule()
{
}

/**
 * @brief Dumps formatted output of the GPIO clock control registers
 */
void ClockModule::dumpModes()
{
    printf( "CLOCK MODULE GPIO MODULE MODES:\n" );
    printf( "   GPIO0_MODULEMODE: %08X\n"
            , mMap.map()->mWkUp.wkup_gpio0_clkctrl.mModuleMode() );
    printf( "   GPIO1_MODULEMODE: %08X\n"
            , mMap.map()->mPer.gpio1_clkctrl.mModuleMode() );
    printf( "   GPIO2_MODULEMODE: %08X\n"
            , mMap.map()->mPer.gpio2_clkctrl.mModuleMode() );
    printf( "   GPIO3_MODULEMODE: %08X\n"
            , mMap.map()->mPer.gpio3_clkctrl.mModuleMode() );
}

/**
 * @brief Enables the clock for the GPIO banks
 */
void ClockModule::enableGpioClocks()
{
    mMap.map()->mWkUp.wkup_gpio0_clkctrl.set_mModuleMode( module_mode_enabled );
    mMap.map()->mPer.gpio1_clkctrl.set_mModuleMode( module_mode_enabled );
    mMap.map()->mPer.gpio2_clkctrl.set_mModuleMode( module_mode_enabled );
    mMap.map()->mPer.gpio3_clkctrl.set_mModuleMode( module_mode_enabled );

}

}
