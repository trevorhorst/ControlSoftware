/** ****************************************************************************
 * @file drivers/am335x/control_module.h
 * @author Trevor Horst
 * @brief Driver for the AM335x control module
 * ****************************************************************************/

#ifndef AM335X_CONTROL_MODULE_H
#define AM335X_CONTROL_MODULE_H

#include "common/control/control_template.h"
#include "common/register.h"

#define CONTROL_MODULE_REVISION_SCHEME( _x_ ) ( ( _x_ & 0xC0000000 ) >> 30 )
#define CONTROL_MODULE_REVISION_FUNC( _x_ ) ( ( _x_ & 0x0FFF0000 ) >> 16 )
#define CONTROL_MODULE_REVISION_RTL( _x_ ) ( ( _x_ & 0x0000F800 ) >> 11 )
#define CONTROL_MODULE_REVISION_MAJOR( _x_ ) ( ( _x_ & 0x00000700 ) >> 8 )
#define CONTROL_MODULE_REVISION_CUSTOM( _x_ ) ( ( _x_ & 0x000000C0 ) >> 6 )
#define CONTROL_MODULE_REVISION_MINOR( _x_ ) ( _x_ & 0x0000003F )

#define CONTROL_MODULE_CONF_MODULE_PIN_SLEWCTRL( _x_ ) ( ( _x_ & 0x00000040 ) >> 6 )
#define CONTROL_MODULE_CONF_MODULE_PIN_RXACTIVE( _x_ ) ( ( _x_ & 0x00000020 ) >> 5 )
#define CONTROL_MODULE_CONF_MODULE_PIN_TYPESEL( _x_ ) ( ( _x_ & 0x00000010 ) >> 4 )
#define CONTROL_MODULE_CONF_MODULE_PIN_PUDEN( _x_ ) ( ( _x_ & 0x00000008 ) >> 3 )
#define CONTROL_MODULE_CONF_MODULE_PIN_MMODE( _x_ ) ( ( _x_ & 0x00000007 ) )

namespace AM335X {

extern const uint32_t addr_control_module;

class ControlModule
        : public ControlTemplate< ControlModule >
{

    struct RegisterMap {
        REGISTER32( Revision )
        {
            REGISTER_FIELD( mData,  mMinor,  0,  6 )
            REGISTER_FIELD( mData, mCustom,  6,  2 )
            REGISTER_FIELD( mData,  mMajor,  8,  3 )
            REGISTER_FIELD( mData,    mRtl, 11,  5 )
            REGISTER_FIELD( mData,   mFunc, 16, 12 )
            REGISTER_FIELD( mData, mScheme, 30,  2 )
        };

        Revision revision;              // 0x0000
        uint32_t hwinfo;                // 0x0004

        uint8_t unused_8[ 0x0008 ];             // 0x0008

        uint32_t sysconfig;             // 0x0010

        uint8_t unused_14[ 0x002C ];            // 0x0014

        uint32_t status;                // 0x0040

        uint8_t unused_44[ 0x00CC ];            // 0x0044

        uint32_t emif_sdram_config;     // 0x0110

        uint8_t unused_114[ 0x06eC ];           // 0x0114

        uint32_t conf_gpmc_ad0;                 // 0x0800
        uint32_t conf_gpmc_ad1                ; // 0x0804
        uint32_t conf_gpmc_ad2                ; // 0x0808
        uint32_t conf_gpmc_ad3                ; // 0x080C
        uint32_t conf_gpmc_ad4                ; // 0x0810
        uint32_t conf_gpmc_ad5                ; // 0x0814
        uint32_t conf_gpmc_ad6                ; // 0x0818
        uint32_t conf_gpmc_ad7                ; // 0x081C
        uint32_t conf_gpmc_ad8                ; // 0x0820
        uint32_t conf_gpmc_ad9                ; // 0x0824
        uint32_t conf_gpmc_ad10               ; // 0x0828
        uint32_t conf_gpmc_ad11               ; // 0x082C
        uint32_t conf_gpmc_ad12               ; // 0x0830
        uint32_t conf_gpmc_ad13               ; // 0x0834
        uint32_t conf_gpmc_ad14               ; // 0x0838
        uint32_t conf_gpmc_ad15               ; // 0x083C
        uint32_t conf_gpmc_a0                 ; // 0x0840
        uint32_t conf_gpmc_a1                 ; // 0x0844
        uint32_t conf_gpmc_a2                 ; // 0x0848
        uint32_t conf_gpmc_a3                 ; // 0x084C
        uint32_t conf_gpmc_a4                 ; // 0x0850
        uint32_t conf_gpmc_a5                 ; // 0x0854
        uint32_t conf_gpmc_a6                 ; // 0x0858
        uint32_t conf_gpmc_a7                 ; // 0x085C
        uint32_t conf_gpmc_a8                 ; // 0x0860
        uint32_t conf_gpmc_a9                 ; // 0x0864
        uint32_t conf_gpmc_a10                ; // 0x0868
        uint32_t conf_gpmc_a11                ; // 0x086C
        uint32_t conf_gpmc_wait0              ; // 0x0870
        uint32_t conf_gpmc_wpn                ; // 0x0874
        uint32_t conf_gpmc_ben1               ; // 0x0878
        uint32_t conf_gpmc_csn0               ; // 0x087C
        uint32_t conf_gpmc_csn1               ; // 0x0880
        uint32_t conf_gpmc_csn2               ; // 0x0884
        uint32_t conf_gpmc_csn3               ; // 0x0888
        uint32_t conf_gpmc_clk                ; // 0x088C
        uint32_t conf_gpmc_advn_ale           ; // 0x0890
        uint32_t conf_gpmc_oen_ren            ; // 0x0894
        uint32_t conf_gpmc_wen                ; // 0x0898
        uint32_t conf_gpmc_ben0_cle           ; // 0x089C
        uint32_t conf_lcd_data0               ; // 0x08A0
        uint32_t conf_lcd_data1               ; // 0x08A4
        uint32_t conf_lcd_data2               ; // 0x08A8
        uint32_t conf_lcd_data3               ; // 0x08AC
        uint32_t conf_lcd_data4               ; // 0x08B0
        uint32_t conf_lcd_data5               ; // 0x08B4
        uint32_t conf_lcd_data6               ; // 0x08B8
        uint32_t conf_lcd_data7               ; // 0x08BC
        uint32_t conf_lcd_data8               ; // 0x08C0
        uint32_t conf_lcd_data9               ; // 0x08C4
        uint32_t conf_lcd_data10              ; // 0x08C8
        uint32_t conf_lcd_data11              ; // 0x08CC
        uint32_t conf_lcd_data12              ; // 0x08D0
        uint32_t conf_lcd_data13              ; // 0x08D4
        uint32_t conf_lcd_data14              ; // 0x08D8
        uint32_t conf_lcd_data15              ; // 0x08DC
        uint32_t conf_lcd_vsync               ; // 0x08E0
        uint32_t conf_lcd_hsync               ; // 0x08E4
        uint32_t conf_lcd_pclk                ; // 0x08E8
        uint32_t conf_lcd_ac_bias_en          ; // 0x08EC
        uint32_t conf_mmc0_dat3               ; // 0x08F0
        uint32_t conf_mmc0_dat2               ; // 0x08F4
        uint32_t conf_mmc0_dat1               ; // 0x08F8
        uint32_t conf_mmc0_dat0               ; // 0x08FC
        uint32_t conf_mmc0_clk                ; // 0x0900
        uint32_t conf_mmc0_cmd                ; // 0x0904
        uint32_t conf_mii1_col                ; // 0x0908
        uint32_t conf_mii1_crs                ; // 0x090C
        uint32_t conf_mii1_rx_er              ; // 0x0910
        uint32_t conf_mii1_tx_en              ; // 0x0914
        uint32_t conf_mii1_rx_dv              ; // 0x0918
        uint32_t conf_mii1_txd3               ; // 0x091C
        uint32_t conf_mii1_txd2               ; // 0x0920
        uint32_t conf_mii1_txd1               ; // 0x0924
        uint32_t conf_mii1_txd0               ; // 0x0928
        uint32_t conf_mii1_tx_clk             ; // 0x092C
        uint32_t conf_mii1_rx_clk             ; // 0x0930
        uint32_t conf_mii1_rxd3               ; // 0x0934
        uint32_t conf_mii1_rxd2               ; // 0x0938
        uint32_t conf_mii1_rxd1               ; // 0x093C
        uint32_t conf_mii1_rxd0               ; // 0x0940
        uint32_t conf_rmii1_ref_clk           ; // 0x0944
        uint32_t conf_mdio                    ; // 0x0948
        uint32_t conf_mdc                     ; // 0x094C
        uint32_t conf_spi0_sclk               ; // 0x0950
        uint32_t conf_spi0_d0                 ; // 0x0954
        uint32_t conf_spi0_d1                 ; // 0x0958
        uint32_t conf_spi0_cs0                ; // 0x095C
        uint32_t conf_spi0_cs1                ; // 0x0960
        uint32_t conf_ecap0_in_pwm0_out       ; // 0x0964
        uint32_t conf_uart0_ctsn              ; // 0x0968
        uint32_t conf_uart0_rtsn              ; // 0x096C
        uint32_t conf_uart0_rxd               ; // 0x0970
        uint32_t conf_uart0_txd               ; // 0x0974
        uint32_t conf_uart1_ctsn              ; // 0x0978
        uint32_t conf_uart1_rtsn              ; // 0x097C
        uint32_t conf_uart1_rxd               ; // 0x0980
        uint32_t conf_uart1_txd               ; // 0x0984
        uint32_t conf_i2c0_sda                ; // 0x0988
        uint32_t conf_i2c0_scl                ; // 0x098C
        uint32_t conf_mcasp0_aclkx            ; // 0x0990
        uint32_t conf_mcasp0_fsx              ; // 0x0994
        uint32_t conf_mcasp0_axr0             ; // 0x0998
        uint32_t conf_mcasp0_ahclkr           ; // 0x099C
        uint32_t conf_mcasp0_aclkr            ; // 0x09A0
        uint32_t conf_mcasp0_fsr              ; // 0x09A4
        uint32_t conf_mcasp0_axr1             ; // 0x09A8
        uint32_t conf_mcasp0_ahclkx           ; // 0x09AC
        uint32_t conf_xdma_event_intr0        ; // 0x09B0
        uint32_t conf_xdma_event_intr1        ; // 0x09B4
        uint32_t conf_warmrstn                ; // 0x09B8

        uint8_t unused_9BC[ 0x0004 ]          ; // 0x09BC

        uint32_t conf_nnmi                    ; // 0x09C0

        uint8_t unused_9C4[ 0x000C ]          ; // 0x09C4

        uint32_t conf_tms                     ; // 0x09D0
        uint32_t conf_tdi                     ; // 0x09D4
        uint32_t conf_tdo                     ; // 0x09D8
        uint32_t conf_tck                     ; // 0x09DC
        uint32_t conf_trstn                   ; // 0x09E0
        uint32_t conf_emu0                    ; // 0x09E4
        uint32_t conf_emu1                    ; // 0x09E8

        uint8_t unused_9EC[ 0x000C ]          ; // 0x09BC

        uint32_t conf_rtc_pwronrstn           ; // 0x09F8
        uint32_t conf_pmic_power_en           ; // 0x09FC
        uint32_t conf_ext_wakeup              ; // 0x0A00

        uint8_t unused_A04[ 0x0018 ]          ; // 0x0A04

        uint32_t conf_usb0_drvvbus            ; // 0x0A1C

        uint8_t unused_A20[ 0x0014 ]          ; // 0x0A20

        uint32_t conf_usb1_drvvbus            ; // 0x0A34

        uint8_t unused_A38[ 0x05C8 ]          ; // 0x0A38
    };

public:
    ControlModule( uint32_t address, bool simulated = false );
    ~ControlModule();
    void dumpRevision();
    void dumpPins();

private:
    MemoryMappedRegister< RegisterMap > mRegister;

};

}

#endif // AM335X_CONTROL_MODULE_H
