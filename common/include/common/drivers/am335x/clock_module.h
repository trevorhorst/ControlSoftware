/** ****************************************************************************
 * @file common/drivers/clock_module.h
 * @author Trevor Horst
 * @brief Driver for the AM335x clock module
 * ****************************************************************************/

#ifndef AM335X_CLOCK_MODULE_H
#define AM335X_CLOCK_MODULE_H

#include "common/control/control_template.h"
#include "common/register.h"

namespace AM335X {

extern const uint32_t addr_clock_module;

class ClockModule
        : public ControlTemplate< ClockModule >
{
    static const uint32_t module_mode_disabled;
    static const uint32_t module_mode_enabled;

    struct CmPer {
        REGISTER32( GpioClkCtrl )
        {
            REGISTER_FIELD( mData, mModuleMode,  0,  2 )
            REGISTER_FIELD( mData,     mIdleSt, 16,  2 )
            REGISTER_FIELD( mData,  mOptFClkEn, 18,  1 )
        };

        uint32_t l4ls_clkstctrl  ;     // 0x0000
        uint32_t l3s_clkstctrl   ;     // 0x0004

        uint8_t unused_8[ 0x0004 ] ;   // 0x0008

        uint32_t l3_clkstctrl    ;     // 0x000C

        uint8_t unused_10[ 0x0004 ];   // 0x0010

        uint32_t cpgmac0_clkctrl ;     // 0x0014

        uint32_t lcdc_clkctrl    ;     // 0x0018
        uint32_t usb0_clkctrl    ;     // 0x001C

        uint8_t unused_20[ 0x0004 ];   // 0x0020

        uint32_t tptc0_clkctrl   ;     // 0x0024
        uint32_t emif_clkctrl    ;     // 0x0028
        uint32_t ocmcram_clkctrl ;     // 0x002C
        uint32_t gpmc_clkctrl    ;     // 0x0030
        uint32_t mcasp0_clkctrl  ;     // 0x0034
        uint32_t uart5_clkctrl   ;     // 0x0038
        uint32_t mmc0_clkctrl    ;     // 0x003C
        uint32_t elm_clkctrl     ;     // 0x0040
        uint32_t i2c2_clkctrl    ;     // 0x0044
        uint32_t i2c1_clkctrl    ;     // 0x0048
        uint32_t spi0_clkctrl    ;     // 0x004C
        uint32_t spi1_clkctrl    ;     // 0x0050

        uint8_t unused_54[ 0x000C ];   // 0x0054

        uint32_t l4ls_clkctrl    ;     // 0x0060

        uint8_t unused_64[ 0x0004 ];   // 0x0064

        uint32_t mcasp1_clkctrl  ;     // 0x0068
        uint32_t uart1_clkctrl   ;     // 0x006C
        uint32_t uart2_clkctrl   ;     // 0x0070
        uint32_t uart3_clkctrl   ;     // 0x0074
        uint32_t uart4_clkctrl   ;     // 0x0078
        uint32_t timer7_clkctrl  ;     // 0x007C
        uint32_t timer2_clkctrl  ;     // 0x0080
        uint32_t timer3_clkctrl  ;     // 0x0084
        uint32_t timer4_clkctrl  ;     // 0x0088

        uint8_t unused_8C[ 0x0020 ];   // 0x008C

        GpioClkCtrl gpio1_clkctrl   ;  // 0x00AC
        GpioClkCtrl gpio2_clkctrl   ;  // 0x00B0
        GpioClkCtrl gpio3_clkctrl   ;  // 0x00B4

        uint8_t unused_B8[ 0x348 ];    // 0x00B8
    };

    struct CmWkup {
        REGISTER32( GpioClkCtrl )
        {
            REGISTER_FIELD( mData, mModuleMode,  0,  2 )
            REGISTER_FIELD( mData,     mIdleSt, 16,  2 )
            REGISTER_FIELD( mData,  mOptFClkEn, 18,  1 )
        };

        uint32_t wkup_clkstctrl;            // 0x0000
        uint32_t wkup_control_clkctrl;      // 0x0004
        GpioClkCtrl wkup_gpio0_clkctrl;     // 0x0008
        uint32_t wkup_l4wkup_clkctrl;       // 0x000C
        uint32_t wkup_timer0_clkctrl;       // 0x0010
        uint32_t wkup_debugss_clkctrl;      // 0x0014
        uint32_t l3_aon_clkstctrl;          // 0x0018
        uint32_t autoidle_dpll_mpu;         // 0x001C
        uint32_t idlest_dpll_mpu;           // 0x0020
        uint32_t ssc_deltamstep_dpll_mpu;   // 0x0024
        uint32_t ssc_modfreqdiv_dpll_mpu;   // 0x0028
        uint32_t clksel_dpll_mpu;           // 0x002C
        uint32_t autoidle_dpll_ddr;         // 0x0030
        uint32_t idlest_dpll_ddr;           // 0x0034
        uint32_t ssc_deltamstep_dpll_ddr;   // 0x0038
        uint32_t ssc_modfreqdiv_dpll_ddr;   // 0x003C
        uint32_t clksel_dpll_ddr;           // 0x0040
        uint32_t autoidle_dpll_disp;        // 0x0044
        uint32_t idlest_dpll_disp;          // 0x0048
        uint32_t ssc_deltamstep_dpll_disp;  // 0x004C
        uint32_t ssc_modfreqdiv_dpll_disp;  // 0x0050
        uint32_t clksel_dpll_disp;          // 0x0054
        uint32_t autoidle_dpll_core;        // 0x0058
        uint32_t idlest_dpll_core;          // 0x005C
        uint32_t ssc_deltamstep_dpll_core;  // 0x0060
        uint32_t ssc_modfreqdiv_dpll_core;  // 0x0064
        uint32_t clksel_dpll_core;          // 0x0068
        uint32_t autoidle_dpll_per;         // 0x006C
        uint32_t idlest_dpll_per;           // 0x0070
        uint32_t ssc_deltamstep_dpll_per;   // 0x0074
        uint32_t ssc_modfreqdiv_dpll_per;   // 0x0078
        uint32_t clkdcoldo_dpll_per;        // 0x007C
        uint32_t div_m4_dpll_core;          // 0x0080
        uint32_t div_m5_dpll_core;          // 0x0084
        uint32_t clkmode_dpll_mpu;          // 0x0088
        uint32_t clkmode_dpll_per;          // 0x008C
        uint32_t clkmode_dpll_core;         // 0x0090
        uint32_t clkmode_dpll_ddr;          // 0x0094
        uint32_t clkmode_dpll_disp;         // 0x0098
        uint32_t clksel_dpll_periph;        // 0x009C
        uint32_t div_m2_dpll_ddr;           // 0x00A0
        uint32_t div_m2_dpll_disp;          // 0x00A4
        uint32_t div_m2_dpll_mpu;           // 0x00A8
        uint32_t div_m2_dpll_per;           // 0x00AC
        uint32_t wkup_wkup_m3_clkctrl;      // 0x00B0
        uint32_t wkup_uart0_clkctrl;        // 0x00B4
        uint32_t wkup_i2c0_clkctrl;         // 0x00B8
        uint32_t wkup_adc_tsc_clkctrl;      // 0x00BC
        uint32_t wkup_smartreflex0_clkctrl; // 0x00C0
        uint32_t wkup_timer1_clkctrl;       // 0x00C4
        uint32_t wkup_smartreflex1_clkctrl; // 0x00C8
        uint32_t l4_wkup_aon_clkstctrl;     // 0x00CC

        uint8_t unused_D0[ 0x0004 ];        // 0x00D0

        uint32_t wkup_wdt1_clkctrl;         // 0x00D4
        uint32_t div_m6_dpll_cor;           // 0x00D8

        uint8_t unused_E0[ 0x0024 ];        // 0x00DC
    };

    struct RegisterMap {
        CmPer mPer;
        CmWkup mWkUp;
    };

public:
    ClockModule( uint32_t address, bool simulated = false );
    ~ClockModule();

    void dumpModes();

    void enableGpioClocks();
private:
    RegisterMap mRegister;
    MemoryMappedRegister< RegisterMap > mMap;
};

}

#endif // AM335X_CLOCK_MODULE_H
