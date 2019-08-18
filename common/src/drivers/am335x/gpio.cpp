#include "common/drivers/am335x/gpio.h"

namespace AM335X {

const uint32_t addr_gpio0_base = 0x44E07000;
const uint32_t addr_gpio1_base = 0x4804C000;
const uint32_t addr_gpio2_base = 0x481AC000;
const uint32_t addr_gpio3_base = 0x481AE000;

const char Gpio::str_input[] = "input";
const char Gpio::str_output[] = "output";

/**
 * @brief Constructor
 * @param address Register address
 */
Gpio::Gpio( uint32_t address, bool simulated )
    : ControlTemplate< Gpio >()
    , mMap( address, simulated )
{
    printf( "GPIO Revision %d.%d\n", mMap.map()->revision.mMajor()
            , mMap.map()->revision.mMinor() );
    printf( "%s: gpio size %lu\n", __FUNCTION__, sizeof( RegisterMap ) );
}

/**
 * @brief Sets the output of the pin
 * @param pin Desired pin on the GPIO bank
 * @param output Desired output state of the pin
 * @return
 */
uint32_t Gpio::setOutput( uint32_t pin, bool output )
{
    uint32_t mask = ( 1 << pin ) | mMap.map()->dataout.mData;
    if( output ) {
        mMap.map()->dataout.set_pin( mask );
    } else {
        mMap.map()->dataout.pin();
    }
    return 0;
}

/**
 * @brief Sets the direction of the pin
 * @param pin Desired pin on the GPIO bank
 * @param direction Desired direction of the pin
 * @return Error code
 */
uint32_t Gpio::setDirection( uint32_t pin, const char *direction )
{
    uint32_t err = Error::Code::NONE;

    uint32_t mask = ( 1 << pin ) | mMap.map()->oe.mData;
    if( direction == nullptr ) {
        err = Error::Code::PARAM_INVALID;
    } else if( strcmp( str_input, direction ) == 0 ) {
        mMap.map()->oe.mData |= mask;
    } else if( strcmp( str_output, direction ) == 0 ) {
        mMap.map()->oe.mData &= ~mask;
    } else {
        err = Error::Code::PARAM_OUT_OF_RANGE;
    }

    return err;
}

/**
 * @brief Gets the data output on the pin
 * @param pin Desired pin on the GPIO bank
 * @return Data output on the pin; true indicate high, false indicates low
 */
bool Gpio::getOutput( uint32_t pin )
{
    uint32_t mask = 1 << pin;
    return mMap.map()->dataout.mData & mask;
}

/**
 * @brief Gets the sampled input data on the pin
 * @param pin Desired pin on the GPIO bank
 * @return Sampled input data; true indicates high, false indicates low
 */
bool Gpio::getInput( uint32_t pin )
{
    uint32_t mask = 1 << pin;
    return mMap.map()->datain.mData & mask;
}

/**
 * @brief Gets the configured direction state of the pin
 * @param pin Desired pin on the GPIO bank
 * @return GPIO pad configuration; true indicates input, false indicates output
 */
const char *Gpio::getDirection( uint32_t pin )
{
    const char *direction = nullptr;
    uint32_t mask = 1 << pin;
    if( mMap.map()->oe.mData & mask ) {
        direction = str_input;
    } else {
        direction = str_output;
    }
    return direction;
}

}

