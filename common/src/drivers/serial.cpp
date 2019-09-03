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

/**
 * @brief Constructor
 */
Serial::Serial( const char* interface , Speed speed )
    : mDone( false )
    , mSpeed( B9600 )
    , fd( 0 )
    , mFileDescriptor( 0 )
{
    // Add an interface name
    if( interface == nullptr || interface[ 0 ] == '\0' ) {
        mInterface[ 0 ] = '\0';
    } else {
        size_t size = sizeof( mInterface );
        strncpy( mInterface, interface, size );
        mInterface[ size - 1 ] = '\0';
    }

    // Configure the speed of the interface
    switch( speed ) {
        case BAUD_110    : mSpeed =   B110; break;
        case BAUD_300    : mSpeed =   B300; break;
        case BAUD_600    : mSpeed =   B600; break;
        case BAUD_1200   : mSpeed =   B1200; break;
        case BAUD_2400   : mSpeed =   B2400; break;
        case BAUD_4800   : mSpeed =   B4800; break;
        case BAUD_9600   : mSpeed =   B9600; break;
        case BAUD_19200  : mSpeed =  B19200; break;
        case BAUD_38400  : mSpeed =  B38400; break;
        case BAUD_57600  : mSpeed =  B57600; break;
        case BAUD_115200 : mSpeed = B115200; break;
        // default: mSpeed = B9600;
    }

    // Open the interface
    openInterface();
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
    struct termios options;                                             // Structure with the device's options

    // Open the device interface
    mFileDescriptor = open( mInterface, O_RDWR | O_NOCTTY | O_NDELAY );
    if( mFileDescriptor == -1 ) {
        error = -2;                                            // If the device is not open, return -1
        LOG_WARN( "%s: Interface failed to open - %s"
                , mInterface, strerror( errno ) );
    } else {

        // Open the device in non blocking mode
        fcntl( mFileDescriptor, F_SETFL, FNDELAY );

        // Set parameters
        // Get the current options of the port
        tcgetattr( mFileDescriptor, &options );
        // Clear all the options
        bzero( &options, sizeof( options ) );

        // Set the input baud rate
        cfsetispeed( &options, mSpeed );
        // Set the output baud rate
        cfsetospeed( &options, mSpeed );

        // Configure the device : 8 bits, no parity, no control
        options.c_cflag |= ( CLOCAL | CREAD |  CS8 );
        // No parity bit
        options.c_cflag &= ~PARENB;
        // Only 1 stop bit
        options.c_cflag &= ~CSTOPB;
        // No hardware flow control
        // options.c_cflag &= ~CRTSCTS;

        // options.c_iflag |= ( IGNPAR | IGNBRK );
        options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON );
        options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN );
        options.c_oflag &= ~OPOST;

        options.c_cc[ VTIME ] = 1;          // Timer unused
        options.c_cc[ VMIN ] = 1;           // At least on character before satisfy reading

        // Activate options
        if( tcsetattr( mFileDescriptor, TCSANOW, &options ) != 0 ) {
            LOG_WARN( "%s: failed to set options - %s"
                , mInterface, strerror( errno ) );
        } else {
            LOG_INFO( "%s: ready", mInterface );
        }
    }

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

int32_t Serial::readBytes( uint8_t *buffer, int32_t size )
{
    int32_t error = 0;

    int32_t bytesRead = 0;

    while( bytesRead < size ) {

        FD_ZERO( &mFileDescriptorSet );
        FD_SET( mFileDescriptor, &mFileDescriptorSet );

        int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet
            , nullptr, nullptr, nullptr );
        if( sel == 1 ) {
            readByte( &buffer[ bytesRead++ ] );
        }
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
    const uint8_t *start, int32_t startSize
    , const uint8_t *stop, int32_t stopSize
    , uint8_t *buffer, int32_t bufferSize )
{
    int32_t error = 0;

    int32_t bytesRead = 0;

    bool startFound = false;
    bool stopFound = false;
    int32_t startIncr = 0;
    int32_t stopIncr = 0;

    while( !stopFound && ( bytesRead < bufferSize ) ) {

        FD_ZERO( &mFileDescriptorSet );
        FD_SET( mFileDescriptor, &mFileDescriptorSet );

        int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet
            , nullptr, nullptr, nullptr );

        if( sel == 1 ) {

            // Read a byte from the buffer
            readByte( &buffer[ bytesRead ] );

            if( startFound ) {
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
                }

                if( stopIncr == stopSize ) {
                    // We have matched all of our stop bytes
                    buffer[ bytesRead ] = '\0';
                    stopFound = true;
                }
            } else {
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


            }
        }
    }

    return error;
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

// _____________________________________
// ::: Read/Write operation on bytes :::

/*!
     \brief Wait for a byte from the serial device and return the data read
     \param pByte : data read on the serial device
     \param TimeOut_ms : delay of timeout before giving up the reading
            If set to zero, timeout is disable (Optional)
     \return 1 success
     \return 0 Timeout reached
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
  */
char Serial::ReadChar(char *pByte,unsigned int TimeOut_ms)
{
#ifdef __linux__
    TimeOut         Timer;                                              // Timer used for timeout
    Timer.InitTimer();                                                  // Initialise the timer
    while (Timer.ElapsedTime_ms()<TimeOut_ms || TimeOut_ms==0)          // While Timeout is not reached
    {
        switch (read(fd,pByte,1)) {                                     // Try to read a byte on the device
        case 1  : return 1;                                             // Read successfull
        case -1 : return -2;                                            // Error while reading
        }
    }
    return 0;
#endif
}



/*!
     \brief Read a string from the serial device (without TimeOut)
     \param String : string read on the serial device
     \param FinalChar : final char of the string
     \param MaxNbBytes : maximum allowed number of bytes read
     \return >0 success, return the number of bytes read
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
     \return -3 MaxNbBytes is reached
  */
int Serial::ReadStringNoTimeOut(char *String,char FinalChar,unsigned int MaxNbBytes)
{
    unsigned int    NbBytes=0;                                          // Number of bytes read
    char            ret;                                                // Returned value from Read
    while (NbBytes<MaxNbBytes)                                          // While the buffer is not full
    {                                                                   // Read a byte with the restant time
        ret=ReadChar(&String[NbBytes]);
        if (ret==1)                                                     // If a byte has been read
        {
            if (String[NbBytes]==FinalChar)                             // Check if it is the final char
            {
                String  [++NbBytes]=0;                                  // Yes : add the end character 0
                return NbBytes;                                         // Return the number of bytes read
            }
            NbBytes++;                                                  // If not, just increase the number of bytes read
        }
        if (ret<0) return ret;                                          // Error while reading : return the error number
    }
    return -3;                                                          // Buffer is full : return -3
}

/*!
     \brief Read a string from the serial device (with timeout)
     \param String : string read on the serial device
     \param FinalChar : final char of the string
     \param MaxNbBytes : maximum allowed number of bytes read
     \param TimeOut_ms : delay of timeout before giving up the reading (optional)
     \return  >0 success, return the number of bytes read
     \return  0 timeout is reached
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
     \return -3 MaxNbBytes is reached
  */
int Serial::ReadString(char *String,char FinalChar,unsigned int MaxNbBytes,unsigned int TimeOut_ms)
{
    if (TimeOut_ms==0)
        return ReadStringNoTimeOut(String,FinalChar,MaxNbBytes);

    unsigned int    NbBytes=0;                                          // Number of bytes read
    char            ret;                                                // Returned value from Read
    TimeOut         Timer;                                              // Timer used for timeout
    long int        TimeOutParam;
    Timer.InitTimer();                                                  // Initialize the timer

    while (NbBytes<MaxNbBytes)                                          // While the buffer is not full
    {                                                                   // Read a byte with the restant time
        TimeOutParam=TimeOut_ms-Timer.ElapsedTime_ms();                 // Compute the TimeOut for the call of ReadChar
        if (TimeOutParam>0)                                             // If the parameter is higher than zero
        {
            ret=ReadChar(&String[NbBytes],TimeOutParam);                // Wait for a byte on the serial link            
            if (ret==1)                                                 // If a byte has been read
            {

                if (String[NbBytes]==FinalChar)                         // Check if it is the final char
                {
                    String  [++NbBytes]=0;                              // Yes : add the end character 0
                    return NbBytes;                                     // Return the number of bytes read
                }
                NbBytes++;                                              // If not, just increase the number of bytes read
            }
            if (ret<0) return ret;                                      // Error while reading : return the error number
        }
        if (Timer.ElapsedTime_ms()>TimeOut_ms) {                        // Timeout is reached
            printf( "timeout\n" );
            String[NbBytes]=0;                                          // Add the end caracter
            return 0;                                                   // Return 0
        }
    }
    return -3;                                                          // Buffer is full : return -3
}

/*!
     \brief Read an array of bytes from the serial device (with timeout)
     \param Buffer : array of bytes read from the serial device
     \param MaxNbBytes : maximum allowed number of bytes read
     \param TimeOut_ms : delay of timeout before giving up the reading
     \return 1 success, return the number of bytes read
     \return 0 Timeout reached
     \return -1 error while setting the Timeout
     \return -2 error while reading the byte
  */
int Serial::Read (void *Buffer,unsigned int MaxNbBytes,unsigned int TimeOut_ms)
{
    TimeOut          Timer;                                             // Timer used for timeout
    Timer.InitTimer();                                                  // Initialise the timer
    unsigned int     NbByteRead=0;
    while (Timer.ElapsedTime_ms()<TimeOut_ms || TimeOut_ms==0)          // While Timeout is not reached
    {
        unsigned char* Ptr=(unsigned char*)Buffer+NbByteRead;           // Compute the position of the current byte
        int Ret=read(fd,(void*)Ptr,MaxNbBytes-NbByteRead);              // Try to read a byte on the device
        if (Ret==-1) return -2;                                         // Error while reading
        if (Ret>0) {                                                    // One or several byte(s) has been read on the device
            NbByteRead+=Ret;                                            // Increase the number of read bytes
            if (NbByteRead>=MaxNbBytes)                                 // Success : bytes has been read
                return 1;
        }
    }
    return 0;                                                           // Timeout reached, return 0
}

// ******************************************
//  Class TimeOut
// ******************************************


/*!
    \brief      Constructor of the class TimeOut.
*/
// Constructor
TimeOut::TimeOut()
{}

/*!
    \brief      Initialise the timer. It writes the current time of the day in the structure PreviousTime.
*/
//Initialize the timer
void TimeOut::InitTimer()
{
    gettimeofday(&PreviousTime, nullptr);
}

/*!
    \brief      Returns the time elapsed since initialization.  It write the current time of the day in the structure CurrentTime.
                Then it returns the difference between CurrentTime and PreviousTime.
    \return     The number of microseconds elapsed since the functions InitTimer was called.
  */
//Return the elapsed time since initialization
unsigned long int TimeOut::ElapsedTime_ms()
{
    struct timeval CurrentTime;
    int sec,usec;
    gettimeofday(&CurrentTime, nullptr);                                   // Get current time
    sec=CurrentTime.tv_sec-PreviousTime.tv_sec;                         // Compute the number of second elapsed since last call
    usec=CurrentTime.tv_usec-PreviousTime.tv_usec;                      // Compute
    if (usec<0) {                                                       // If the previous usec is higher than the current one
        usec=1000000-PreviousTime.tv_usec+CurrentTime.tv_usec;          // Recompute the microseonds
        sec--;                                                          // Substract one second
    }
    return sec*1000+usec/1000;
}

