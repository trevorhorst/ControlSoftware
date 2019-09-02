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
 * @brief Open the serial port
 * @param Device : Port name (COM1, COM2, ... for Windows ) or (/dev/ttyS0, /dev/ttyACM0, /dev/ttyUSB0 ... for linux)
 * @param Bauds : Baud rate of the serial port.
 *
 *           \n Supported baud rate for Windows :
 *                   - 110
 *                   - 300
 *                   - 600
 *                   - 1200
 *                   - 2400
 *                   - 4800
 *                   - 9600
 *                   - 14400
 *                   - 19200
 *                   - 38400
 *                   - 56000
 *                   - 57600
 *                   - 115200
 *                   - 128000
 *                   - 256000
 *
 *          \n Supported baud rate for Linux :\n
 *                   - 110
 *                   - 300
 *                   - 600
 *                   - 1200
 *                   - 2400
 *                   - 4800
 *                   - 9600
 *                   - 19200
 *                   - 38400
 *                   - 57600
 *                   - 115200
 *
 * @return 1 success
 * @return -1 device not found
 * @return -2 error while opening the device
 * @return -3 error while getting port parameters
 * @return -4 Speed (Bauds) not recognized
 * @return -5 error while writing port parameters
 * @return -6 error while writing timeout parameters
 */

int32_t Serial::openInterface()
{
    int32_t error = 0;
    struct termios options;                                             // Structure with the device's options

    // Open the device interface
    fd = open( mInterface, O_RDWR | O_NOCTTY | O_NDELAY );
    if( fd == -1 ) {
        error = -2;                                            // If the device is not open, return -1
        printf( "%s: Interface failed to open - %s\n"
                , mInterface, strerror( errno ) );
    } else {

        // Open the device in non blocking mode
        // fcntl( fd, F_SETFL, FNDELAY );

        // Set parameters
        // Get the current options of the port
        tcgetattr( fd, &options );
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
        if( tcsetattr( fd, TCSANOW, &options ) != 0 ) {
            printf( "%s: Failed to set options\n" );
        } else {
            printf( "%s: Ready\n", mInterface );
        }
    }

    return error;
}


/**
 * @brief Closes the serial interface
 */
void Serial::closeInterface()
{
    if( close( fd ) < 0 ) {
        printf( "%s: Failed to close - %s\n"
                , mInterface, strerror( errno ) );
    } else {
        printf( "%s: Closed - %s\n"
                , mInterface, strerror( errno ) );
    }
}


int32_t Serial::readInterface()
{
    int32_t error = 0;

    char inputBuffer[ 2048 ];
    inputBuffer[ 0 ] = 0;
    // char str[]       = "\xA0\xA1\x00\x02\x02\x00\x02\x0D\x0A";
    unsigned char str[] = "\xA0\xA1\x00\x03\x09\x00\x09\x0D\x0A";
    FD_ZERO( &mFileDescriptorSet );
    FD_SET( fd, &mFileDescriptorSet );

    FlushReceiver();
    tcflush(fd,TCOFLUSH);
    // for( int i = 0; i < 10; i++ ) {
    //     // printf( "0x%02X ", str[ i ] );
    //     WriteChar( str[ i ] );
    // }
    write( fd, str, 10 );
    // usleep( 50000 );
    printf( "\n" );
    char buffer[ 128 ];
    readSegment( buffer, 128 );
    if( buffer[ 4 ] == '\x83' ) {
        printf( "Ack Received\n" );
        readSegment( buffer, 128 );
        if( buffer[ 4 ] == '\x83' ) {
            printf( "Ack Received\n" );
            readSegment( buffer, 128 );
        } else {
            printf( "Nack Received\n" );
        }
    }


    // Write( str, 10 );
    // usleep( 50000 );
    // int32_t size = 100;
    // char readback[ size ];
    // for( int i = 0; i < size; i++ ) {
    //     readback[ i ] = 0;
    // }

    // ReadString( readback, '\x0A', size, 0 );
    // read( fd, readback, size );
    // for( int i = 0; i < size; i++ ) {
    //     printf( "0x%02X ", static_cast< u_char >( readback[ i ] ) );
    // }
    // printf( "\nReadBack: %s\n", readback );
    while( !mDone ) {
        // printf( "Select triggered: ReadString\n" );
        int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet, nullptr, nullptr, nullptr );
        if( sel == 1 ) {
            // usleep( 250000 );
            // ReadString( inputBuffer, '\n', 1024, 5000 );
            inputBuffer[ 0 ] = 0;
            ssize_t bytes = read( fd, inputBuffer, 2048 );
            inputBuffer[ bytes ] = 0;
            // ReadString( inputBuffer, '\n', 1024, 0 );
            printf( "%s", inputBuffer );
        } else {
            error = -1;
        }
    }

    return error;
}

/*!
     \brief Write a char on the current serial port
     \param Byte : char to send on the port (must be terminated by '\0')
     \return 1 success
     \return -1 error while writting data
  */
char Serial::WriteChar(const char Byte)
{
    if (write(fd,&Byte,1)!=1)                                           // Write the char
        return -1;                                                      // Error while writting
    return 1;                                                           // Write operation successfull
}



//________________________________________
// ::: Read/Write operation on strings :::


/*!
     \brief Write a string on the current serial port
     \param String : string to send on the port (must be terminated by '\0')
     \return 1 success
     \return -1 error while writting data
  */
char Serial::WriteString(const char *String)
{
#ifdef __linux__
    int Lenght=strlen(String);                                          // Lenght of the string
    if (write(fd,String,Lenght)!=Lenght)                                // Write the string
        return -1;                                                      // error while writing
    return 1;                                                           // Write operation successfull
#endif
}

// _____________________________________
// ::: Read/Write operation on bytes :::



/*!
     \brief Write an array of data on the current serial port
     \param Buffer : array of bytes to send on the port
     \param NbBytes : number of byte to send
     \return 1 success
     \return -1 error while writting data
  */
char Serial::Write(const void *Buffer, const unsigned int NbBytes)
{
    if( write( fd, Buffer, NbBytes ) != (ssize_t)NbBytes ) {                              // Write data
        return -1;
    } // Error while writing

    return 1;                                                           // Write operation successfull
}



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

int32_t Serial::readSegment( char *buffer, int32_t size )
{
    int32_t error = 0;

    for( int32_t i = 0; i < size; i++ ) {
        buffer[ i ] = 0;
    }
    int32_t sequenceSize = 2;
    char startSequence[] = { '\xA0', '\xA1' };
    char endSequence[] = { '\x0D', '\x0A' };

    bool done = false;
    while( !done ) {
        readChar( &buffer[ 0 ] );
        if( buffer[ 0 ] == startSequence[ 0 ] ) {
            readChar( &buffer[ 1 ] );
            if( buffer[ 1 ] == startSequence[ 1 ] ) {
                // read( fd, &buffer[ 2 ], strlen( startSequence ) );
                printf( "Found start sequence!\n" );
                bool messageDone = false;
                int32_t index = sequenceSize;
                while( !messageDone ) {
                    readChar( &buffer[ index ]);
                    if( buffer[ index ] == endSequence[ 0 ] ) {
                        readChar( &buffer[ index + 1 ] );
                        if( buffer[ index + 1 ] == endSequence[ 1 ] ) {
                            printf( "Found end sequence!\n" );
                            messageDone = true;
                        }
                    }
                    index++;
                }
                done = true;
            }
        }
    }

    for( int32_t i = 0; i < size; i++ ) {
        printf( "0x%02X ", static_cast< unsigned char >( buffer[ i ] ) );
    }
    printf( "\n" );

    return error;
}

int32_t Serial::readChar(char *byte)
{
    int32_t error = 0;

    int32_t sel = select( FD_SETSIZE, &mFileDescriptorSet, nullptr, nullptr, nullptr );
    if( sel == 1 ) {
        read( fd, byte, 1 );
    } else {
        error = -1;
    }

    return error;
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




// _________________________
// ::: Special operation :::



/*!
    \brief Empty receiver buffer (UNIX only)
*/

void Serial::FlushReceiver()
{
#ifdef __linux__
    tcflush(fd,TCIFLUSH);
#endif
}



/*!
    \brief  Return the number of bytes in the received buffer (UNIX only)
    \return The number of bytes in the received buffer
*/
int Serial::Peek()
{
    int Nbytes=0;
#ifdef __linux__
    ioctl(fd, FIONREAD, &Nbytes);
#endif
    return Nbytes;
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
    gettimeofday(&PreviousTime, NULL);
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
    gettimeofday(&CurrentTime, NULL);                                   // Get current time
    sec=CurrentTime.tv_sec-PreviousTime.tv_sec;                         // Compute the number of second elapsed since last call
    usec=CurrentTime.tv_usec-PreviousTime.tv_usec;                      // Compute
    if (usec<0) {                                                       // If the previous usec is higher than the current one
        usec=1000000-PreviousTime.tv_usec+CurrentTime.tv_usec;          // Recompute the microseonds
        sec--;                                                          // Substract one second
    }
    return sec*1000+usec/1000;
}

