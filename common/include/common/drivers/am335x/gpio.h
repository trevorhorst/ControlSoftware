#ifndef AM335X_GPIO_H
#define AM335X_GPIO_H

#include "common/control/control_template.h"
#include "common/register.h"

namespace AM335X {

extern const uint32_t addr_gpio0_base;
extern const uint32_t addr_gpio1_base;
extern const uint32_t addr_gpio2_base;
extern const uint32_t addr_gpio3_base;

class Gpio
        : public ControlTemplate< Gpio >
{
    static const uint32_t pins_per_bank;

    /**
     * @brief The general purpose input output register struct
     */
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

        REGISTER32( DataIn )
        {
            REGISTER_FIELD( mData, mPin, 0, 32 )
        };

        REGISTER32( DataOut )
        {
            REGISTER_FIELD( mData, pin, 0, 32 )
        };

        REGISTER32( OE )
        {
            REGISTER_FIELD( mData, pin, 0, 32 )
        };

        Revision revision;              // 0x0000 GPIO Revision - Read Only

        uint8_t unused_4[ 0x000C ];     // 0x0004 Unused

        uint32_t sysconfig;             // 0x0010 GPIO Sysconfig

        uint8_t unused_14[ 0x000C ];    // 0x0014 Unused

        uint32_t eoi;                   // 0x0020 GPIO EOI

        uint32_t irqstatus_raw_0;       // 0x0024 Interrupt handling
        uint32_t irqstatus_raw_1;       // 0x0028 Interrupt handling
        uint32_t irqstatus_0;           // 0x002C Interrupt handling
        uint32_t irqstatus_1;           // 0x0030 Interrupt handling
        uint32_t irqstatus_set_0;       // 0x0034 Interrupt handling
        uint32_t irqstatus_set_1;       // 0x0038 Interrupt handling
        uint32_t irqstatus_clr_0;       // 0x003C Interrupt handling
        uint32_t irqstatus_clr_1;       // 0x0040 Interrupt handling
        uint32_t irqwaken_0;            // 0x0044 Interrupt handling
        uint32_t irqwaken_1;            // 0x0048 Interrupt handling

        uint8_t unused_4C[ 0x00C8 ];    // 0x004C Unused

        uint32_t sysstatus;             // 0x0114 GPIO status

        uint8_t unused_118[ 0x0018 ];   // 0x0118 Unused

        uint32_t ctrl;                  // 0x0130 Clock gating
        OE       oe;                    // 0x0134 Output capabilities
        DataIn   datain;                // 0x0138
        DataOut  dataout;               // 0x013C
        uint32_t leveldetect0;          // 0x0140
        uint32_t leveldetect1;          // 0x0144
        uint32_t risingdetect;          // 0x0148
        uint32_t fallingdetect;         // 0x014C
        uint32_t debouncenable;         // 0x0150
        uint32_t debouncingtime;        // 0x0154

        uint8_t unused_158[ 0x0038 ];   // 0x0158

        uint32_t cleardataout;          // 0x0190
        uint32_t setdataout;            // 0x0194

        uint8_t unused_198[ 0x0E68 ];   // 0x0198 Fill in the remaining space
    };

public:
    static const uint32_t banks;
    static const char str_input[];
    static const char str_output[];

    Gpio( uint32_t addr, bool simulated = false );
    // ~Gpio();

    void dumpRevision();


    bool getOutput( uint32_t pin );
    bool getInput( uint32_t pin );

    const char *getDirection( uint32_t pin );

    uint32_t getOutput();
    uint32_t getInput();

    uint32_t setOutput( uint32_t pin, bool output );
    uint32_t setDirection( uint32_t pin, const char *direction );

private:
    MemoryMappedRegister< RegisterMap > mRegister;
};

}

#endif // AM335X_GPIO_H
