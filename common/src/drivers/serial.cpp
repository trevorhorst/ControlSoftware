/*!
 \file    Serial.cpp
 \brief   Class to manage the serial port
 \author  Philippe Lucidarme (University of Angers) <Serial@googlegroups.com>
 \version 1.2
 \date    28 avril 2011

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


This is a licence-free software, it can be used by anyone who try to build a better world.
 */

#include "common/drivers/serial.h"

const uint32_t Serial::set_speed = 1 << 0;

/**
 * @brief Constructor
 */
Serial::Serial( const char* interface , Speed speed, bool simulated )
    : mSimulated( simulated )
    , mFileDescriptor( -1 )
    , mSpeed( B0 )
{
    // Add an interface name
    if( interface == nullptr || interface[ 0 ] == '\0' ) {
        mInterface[ 0 ] = '\0';
    } else {
        size_t size = sizeof( mInterface );
        strncpy( mInterface, interface, size );
        mInterface[ size - 1 ] = '\0';
    }

    // Open the interface, configure settings and apply
    openInterface();
    setInterfaceSpeed( speed );
    applySettings();
}


/**
 * @brief Destructor
 */
Serial::~Serial()
{
    closeInterface();
}


/**
 * @brief Opens the serial port interface
 * @return int32_t error code
 */
int32_t Serial::openInterface()
{
    int32_t error = 0;

    if( isInterfaceOpen() ) {
        // Close the interface if it is already open
        closeInterface();
    }

    // Open the device interface
    mFileDescriptor = open( mInterface, O_RDWR | O_NOCTTY | O_NDELAY );
    if( mFileDescriptor == -1 ) {
        error = -2;                                            // If the device is not open, return -1
        LOG_WARN( "%s: interface failed to open - %s"
                , mInterface, strerror( errno ) );
    } else {
        LOG_INFO( "%s: interface ready", mInterface );
    }

    return error;
}

uint32_t Serial::getInterfaceSpeed()
{
    termios options;

    speed_t speed = B0;

    if( isInterfaceOpen() ) {
        tcgetattr( mFileDescriptor, &options );
        speed = cfgetospeed( &options );
        switch( speed ) {
            case    B110: speed = BAUD_110; break;
            case    B300: speed = BAUD_300; break;
            case    B600: speed = BAUD_600; break;
            case   B1200: speed = BAUD_1200; break;
            case   B2400: speed = BAUD_2400; break;
            case   B4800: speed = BAUD_4800; break;
            case   B9600: speed = BAUD_9600; break;
            case  B19200: speed = BAUD_19200; break;
            case  B38400: speed = BAUD_38400; break;
            case  B57600: speed = BAUD_57600; break;
            case B115200: speed = BAUD_115200; break;
        }
    }

    return speed;
}

int32_t Serial::setInterfaceSpeed( Speed speed )
{
    int32_t error = 0;

    // Configure the speed of the interface
    switch( speed ) {
        case BAUD_110    : mSettings.speed =   B110; break;
        case BAUD_300    : mSettings.speed =   B300; break;
        case BAUD_600    : mSettings.speed =   B600; break;
        case BAUD_1200   : mSettings.speed =   B1200; break;
        case BAUD_2400   : mSettings.speed =   B2400; break;
        case BAUD_4800   : mSettings.speed =   B4800; break;
        case BAUD_9600   : mSettings.speed =   B9600; break;
        case BAUD_19200  : mSettings.speed =  B19200; break;
        case BAUD_38400  : mSettings.speed =  B38400; break;
        case BAUD_57600  : mSettings.speed =  B57600; break;
        case BAUD_115200 : mSettings.speed = B115200; break;
    }

    mSettings.mask |= set_speed;

    return error;
}

/**
 * @brief Closes the serial interface
 */
void Serial::closeInterface()
{
    if( close( mFileDescriptor ) < 0 ) {
        LOG_WARN( "%s: failed to close - %s"
                , mInterface, strerror( errno ) );
    } else {
        LOG_INFO( "%s: closed", mInterface );
    }
}

bool Serial::isInterfaceOpen()
{
    bool open = false;
    if( mFileDescriptor > 0 ) {
        open = true;
    }
    return open;
}

/**
 * @brief Flushes the serial interface receiver
 */
void Serial::flushReceiver()
{
    if( tcflush( mFileDescriptor, TCIFLUSH ) < 0 ) {
        LOG_WARN( "%s: failed to flush receiver - %s"
            , mInterface, strerror( errno ) );
    }
}

/**
 * @brief Flushed the serial interface transmitter
 * 
 */
void Serial::flushTransmitter()
{
    if( tcflush( mFileDescriptor, TCOFLUSH ) < 0 ) {
        LOG_WARN( "%s: failed to flush transmitter - %s"
            , mInterface, strerror( errno ) );
    }
}

/**
 * @brief Determine the number of bytes available on the receiver
 * @return int32_t bytes
 */
int32_t Serial::availableBytes()
{
    int32_t bytes = 0;
    ioctl( mFileDescriptor, FIONREAD, &bytes );
    return bytes;
}

/**
 * @brief Read a byte from the interface
 * @param buffer Buffer to store data
 * @return int32_t error code
 */
int32_t Serial::readByte( uint8_t *buffer )
{
    int32_t error = 0;

    mMutex.lock();

    if( read( mFileDescriptor, buffer, 1 ) != 1 ) {
        error = -1;
    }

    mMutex.unlock();

    return error;
}

/**
 * @brief Read several bytes from th interface
 * @param buffer Buffer to store the data
 * @param size Size of the buffer to store the data
 * @return
 */
int32_t Serial::readBytes( uint8_t *buffer, int32_t size )
{
    int32_t error = 0;

    int32_t bytesRead = 0;

    if( !isInterfaceOpen() ) {
        error = -1;
    } else {
        mMutex.lock();
        while( error > 0
               && bytesRead < size ) {

            FD_ZERO( &mFileDescriptorSet );
            FD_SET( mFileDescriptor, &mFileDescriptorSet );

            int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet
                , nullptr, nullptr, nullptr );
            if( sel == 1 ) {
                readByte( &buffer[ bytesRead++ ] );
            }
        }
        mMutex.unlock();
    }

    return error;
}

/**
 * @brief Reads a specified pattern from the interface
 * @param start Start sequence
 * @param startSize Size of the start sequence
 * @param stop Stop sequence
 * @param stopSize Size of the stop sequence
 * @param buffer Buffer to store data
 * @param bufferSize Size of the storage buffer
 * @return int32_t error code
 */
int32_t Serial::readPattern( 
    const uint8_t *start, uint32_t startSize
    , const uint8_t *stop, uint32_t stopSize
    , uint8_t *buffer, uint32_t bufferSize )
{
    int32_t error = 0;

    bool startFound = false;
    bool stopFound = false;
    uint32_t startIncr = 0;
    uint32_t stopIncr = 0;
    uint32_t bytesRead = 0;

    Timeout timer;

    if( !isInterfaceOpen() ) {
        error = -1;
    } else {
        timer.init();
    }

    while( error >= 0
           && timer.elapsedTime() <= 5000
           && !stopFound
           && ( bytesRead < bufferSize ) ) {

        FD_ZERO( &mFileDescriptorSet );
        FD_SET( mFileDescriptor, &mFileDescriptorSet );

        timeval timeout = { 0, 10000 };
        int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet
            , nullptr, nullptr, &timeout );

        if( sel == 1 ) {
            // Read a byte from the buffer
            error = readByte( &buffer[ bytesRead ] );

            if( error < 0 ) {
                LOG_ERROR( "%s: read pattern error - %s"
                           , mInterface, strerror( errno ) );
            } else if( !startFound ) {
                // The start byte hasn't been found, look for the start
                if( buffer[ bytesRead ] == start[ startIncr ] ) {
                    // Our current byte matches our start byte, increment both
                    startIncr++;
                    bytesRead++;
                } else {
                    // Our current byte does not match our start byte, reset the
                    // increment and the bytes read
                    startIncr = 0;
                    bytesRead = 0;
                }

                if( startIncr == startSize ) {
                    // We have matched all of our start bytes
                    startFound = true;
                }

            } else {
                // The start has been found, look for the stop
                if( buffer[ bytesRead ] == stop[ stopIncr ] ) {
                    // Our current byte matches our stop byte, increment both
                    stopIncr++;
                    bytesRead++;
                } else {
                    // Our current byte does not match our stop byte, reset
                    // the increment
                    stopIncr = 0;
                    bytesRead++;
                    /// @todo In some cases we may want to ignore empty bytes
                    /// may need to come up with a separate method
                    // if( buffer[ bytesRead ] != 0 ) {
                    //     bytesRead++;
                    // }
                }

                if( stopIncr == stopSize ) {
                    // We have matched all of our stop bytes
                    buffer[ bytesRead ] = '\0';
                    stopFound = true;
                }

            }
        }
    }

    return ( error >= 0 ) ? static_cast< int32_t >( bytesRead ) : error;
}

/**
 * @brief Write bytes to the serial interface
 * @param buffer Buffer to write
 * @param size Size of the buffer to write
 * @return int32_t error code
 */
int32_t Serial::writeBytes( const uint8_t *buffer, uint32_t size )
{
    int32_t error = 0;

    if( mFileDescriptor <= 0 ) {
        error = -1;
    } else {
        ssize_t bytesWritten = write( mFileDescriptor, buffer, size );
        if( bytesWritten != size ) {
            LOG_WARN( "%s: only wrote %d bytes, expected %d", bytesWritten, size );
            error = -1;
        }
    }

    return error;
}

/**
 * @brief Apply settings to the interface, This method must be called in order
 * to apply settings
 * @return int32_t error code
 */
int32_t Serial::applySettings()
{
    int32_t error = 0;

    // Get the current terminal settings
    termios tty;

    if( !isInterfaceOpen() ) {
        error = -1;
    } else {
        // Only get the terminal attributes if the terminal is open
        error = tcgetattr( mFileDescriptor, &tty );
        // bzero( &tty, sizeof( tty ) );
    }

    if( error == 0 && ( mSettings.mask & set_speed ) ) {
        // Set the input baud rate
        error = cfsetispeed( &tty, mSettings.speed );
        if( error != 0 ) {
            LOG_WARN( "%s: failed to set input speed - %s"
                      , mInterface, strerror( errno ) );
        }
    }

    if( error == 0 && ( mSettings.mask & set_speed ) ) {
        // Set the output baud rate
        error = cfsetospeed( &tty, mSettings.speed );
        if( error != 0 ) {
            LOG_WARN( "%s: failed to set output speed - %s"
                      , mInterface, strerror( errno ) );
        }
    }

    if( error == 0 ) {
        // Configure the device : 8 bits, no parity, no control
        tty.c_cflag |= ( CLOCAL | CREAD |  CS8 );
        // No parity bit
        tty.c_cflag &= ~PARENB;
        // Only 1 stop bit
        tty.c_cflag &= ~CSTOPB;
        // No hardware flow control
        tty.c_cflag &= ~CRTSCTS;

        // tty.c_iflag |= ( IGNPAR | IGNBRK );
        tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON );
        tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN );
        tty.c_oflag &= ~OPOST;

        tty.c_cc[ VTIME ] = 1;          // Timeout unused
        tty.c_cc[ VMIN ] = 1;           // At least on character before satisfy reading

        // Activate mOptions
        error = tcsetattr( mFileDescriptor, TCSANOW, &tty );
        if( error != 0 ) {
            LOG_WARN( "%s: failed to set attributes - %s"
                , mInterface, strerror( errno ) );
        } else {
            LOG_INFO( "%s: configured", mInterface );
        }
    }

    mSettings.mask = 0;

    return error;
}

/**
 * @brief Constructor
 */
Timeout::Timeout()
    : mPreviousTime{ 0, 0 }
{

}

/**
 * @brief Destructor
 */
void Timeout::init()
{
    // Initialize the timer. It writes the current time of the day in the
    gettimeofday( &mPreviousTime, nullptr );
}

/**
 * @brief Returns the time elapsed since initialization.  It write the current
 * time of the day in the structure CurrentTime. Then it returns the difference
 * between CurrentTime and PreviousTime.
 * @return The number of microseconds elapsed since the functions InitTimeout was called.
 */
unsigned long int Timeout::elapsedTime()
{
    struct timeval currentTime;
    long sec = 0;
    long usec = 0;
    // Get the current time
    gettimeofday( &currentTime, nullptr );
    // Compute the number of elapsed time since the last call
    sec = currentTime.tv_sec - mPreviousTime.tv_sec;
    usec = currentTime.tv_usec - mPreviousTime.tv_usec;
    // If the previous usec is higher than the current one
    if( usec < 0 ) {
        // Recompute the microseconds
        usec = 1000000 - mPreviousTime.tv_usec + currentTime.tv_usec;
        // Subtract one second
        sec--;
    }
    return static_cast< unsigned long >( sec * 1000 + usec / 1000 );
}

