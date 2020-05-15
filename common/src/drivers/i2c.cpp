#include "common/drivers/i2c.h"

const uint32_t I2C::set_slave = 1 << 0;

/**
 * @brief Constructor
 * @param deviceAddr Device addres
 * @param control
 */
I2C::I2C( const char *device, uint8_t deviceAddr, uint8_t control )
    : mError( false )
    , mAddress( deviceAddr )
    , mControl( control )
    , mFileDescriptor( -1 )
{
    if( device == nullptr || device[ 0 ] == '\0' ) {
        mDevice[ 0 ] = '\0';
    } else {
        size_t size = sizeof( mDevice );
        strncpy( mDevice, device, size );
        mDevice[ size - 1 ] = '\0';
    }

    openDevice();
}

/**
 * @brief Destructor
 */
I2C::~I2C()
{
    closeDevice();
}

/**
 * @brief Open the device
 * @return int32_t error code
 */
int32_t I2C::openDevice()
{
    int32_t error = 0;

    if( isDeviceOpen() ) {
        closeDevice();
    }

    mFileDescriptor = open( mDevice, O_RDWR );
    if( mFileDescriptor == -1 ) {
        error = -2;
        LOG_WARN( "%s: device failed to open - %s"
                , mDevice, strerror( errno ) );
    } else {
        LOG_INFO( "%s: device ready", mDevice );
       // if( ioctl( mFileDescriptor, I2C_SLAVE, mAddress ) < 0 ) {
       //     LOG_ERROR( "could not find device on address: %d", mAddress );
       // }
    }

    return error;
}


/**
 * @brief Close the device
 */
void I2C::closeDevice()
{
    if( close( mFileDescriptor ) < 0 ) {
        LOG_WARN( "%s: failed to close - %s"
                , mDevice, strerror( errno ) );
    } else {
        LOG_INFO( "%s: closed", mDevice );
    }
}

/**
 * @brief Retrieves the status of the interface
 * @return
 */
bool I2C::isDeviceOpen()
{
    bool open = false;
    if( mFileDescriptor > 0 ) {
        open = true;
    }
    return open;
}

int32_t I2C::setSlave( uint8_t slave )
{
    int32_t error = 0;

    mSettings.slave = slave;
    mSettings.mask |= set_slave;

    return error;
}

/**
 * @brief Read bytes from the device
 * @param buffer Buffer to read into
 * @param size Size of the buffer to read into
 * @return int32_t error code
 */
int32_t I2C::readBytes( uint8_t *buffer, int32_t size )
{
    int32_t error = 0;

    int32_t bytesRead = 0;

    if( !isDeviceOpen() ) {
        error = -1;
    } else {
        while( ( error > 0 )
               && ( bytesRead < size ) ) {

            FD_ZERO( &mFileDescriptorSet );
            FD_SET( mFileDescriptor, &mFileDescriptorSet );

            int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet
                , nullptr, nullptr, nullptr );
            if( sel == 1 ) {
                bytesRead +=
                    read( mFileDescriptor, &buffer[ bytesRead ], size - bytesRead );
            }
        }
    }

    return error;

}

/**
 * @brief Write bytes to the device
 * @param buffer Buffer to write from
 * @param size Size of the buffer to write from
 * @return int32_t error code
 */
int32_t I2C::writeBytes( const uint8_t *buffer, uint32_t size )
{
    int32_t error = 0;

    if( !isDeviceOpen() ) {
        error = -1;
    } else {
        ssize_t bytesWritten = write( mFileDescriptor, buffer, size );
        if( bytesWritten != size ) {
            LOG_WARN( "%s: only wrote %d bytes, expected %d - %s"
                      , mDevice, bytesWritten, size, strerror( errno ) );
            error = -1;
        }
    }

    return error;
}

int32_t I2C::applySettings()
{
    int32_t error = 0;

    if( !isDeviceOpen() ) {
        error = -1;
    }

    if( error == 0 && ( mSettings.mask & set_slave ) ) {
        error = ioctl( mFileDescriptor, I2C_SLAVE, mSettings.slave );
        if( error < 0 ) {
            LOG_ERROR( "%s: error setting slave %d - %s"
                       , mDevice, mSettings.slave, strerror( errno ) );
        }
    }

    mSettings.mask = 0;

    return error;
}
