#ifndef I2C_H
#define I2C_H

#include <sys/ioctl.h>
#include <cstdint>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "common/logger/log.h"

#define DEVICE_NAME_MAX_SIZE 64

class I2C
{
    struct Settings {
        uint32_t mask;
        uint8_t slave;
    };

    static const uint32_t set_slave;

public:
    I2C( const char *device, uint8_t deviceAddr, uint8_t control );
    ~I2C();

    void closeDevice();
    int32_t openDevice();
    bool isDeviceOpen();

    bool writeCommand( int numBytesToWrite, ...);
    int32_t writeBytes( const uint8_t *buffer, uint32_t size );
    int32_t readBytes( uint8_t *data, int count );

    int32_t setSlave( uint8_t slave );

    int32_t applySettings();

private:

    bool mError;
    char mDevice[ DEVICE_NAME_MAX_SIZE ];
    uint8_t mAddress;
    uint8_t mControl;
    int32_t mFileDescriptor;
    fd_set mFileDescriptorSet;
    Settings mSettings;
};

#endif // I2C_H
