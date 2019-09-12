/*!
\file    Serial.h
\brief   Serial library to communicate throught serial port, or any device emulating a serial port.
\author  Philippe Lucidarme (University of Angers) <Serial@googlegroups.com>
\version 1.2
\date    28 avril 2011
This Serial library is used to communicate through serial port.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is a licence-free software, it can be used by anyone who try to build a better world.
*/

#ifndef SERIALIB_H
#define SERIALIB_H

// Used for Timeout operations
#include <sys/time.h>

// Include for Linux
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
#include <iostream>
// File control definitions
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <mutex>

#include "common/logger/log.h"

#define INTERFACE_NAME_MAX_SIZE 64

class Serial
{
    struct Settings {
        uint32_t mask;
        speed_t speed;
    };

    static const uint32_t set_speed;

public:

    enum Speed {
        BAUD_110      = 110
        , BAUD_300    = 300
        , BAUD_600    = 600
        , BAUD_1200   = 1200
        , BAUD_2400   = 2400
        , BAUD_4800   = 4800
        , BAUD_9600   = 9600
        , BAUD_19200  = 19200
        , BAUD_38400  = 38400
        , BAUD_57600  = 57600
        , BAUD_115200 = 115200
    };

    Serial( const char *interface, Speed speed );
    ~Serial();

    void closeInterface();
    int32_t openInterface();

    uint32_t getInterfaceSpeed();
    int32_t setInterfaceSpeed( Speed speed );

    int32_t applySettings();

    void flushReceiver();
    void flushTransmitter();

    bool isInterfaceOpen();

    int32_t availableBytes();

    int32_t readByte( uint8_t *buffer );
    int32_t readBytes( uint8_t *buffer, int32_t size );
    int32_t readPattern(
            const uint8_t *start, uint32_t startSize
            , const uint8_t *stop, uint32_t stopSize
            , uint8_t *buffer, uint32_t bufferSize );

    int32_t writeBytes( const uint8_t *buffer, uint32_t size );

private:

    bool mDone;

    char mInterface[ INTERFACE_NAME_MAX_SIZE ];

    int32_t mFileDescriptor;

    Settings mSettings;

    fd_set mFileDescriptorSet;
    uint32_t mSpeed;

    termios mOptions;
    std::mutex mMutex;

};

class Timeout
{
public:
    Timeout();

    void init();
    unsigned long int elapsedTime();

private:    
    struct timeval mPreviousTime;
};

#endif // SERIALIB_H
