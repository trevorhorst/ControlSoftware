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

// Used for TimeOut operations
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

    void setDone() { mDone = true; }
    void closeInterface();
    int32_t openInterface();

    void flushReceiver();
    void flushTransmitter();

    int32_t availableBytes();

    int32_t readByte( uint8_t *buffer );
    int32_t readBytes( uint8_t *buffer, int32_t size );
    int32_t readPattern(
            const uint8_t *start, int32_t startSize
            , const uint8_t *stop, int32_t stopSize
            , uint8_t *buffer, int32_t bufferSize );

    int32_t writeBytes( const uint8_t *buffer, uint32_t size );

////////////////////////////////////////////////////////////////////////////////
// Old Methods 
////////////////////////////////////////////////////////////////////////////////

    // Read a char (with timeout)
    char    ReadChar    (char *pByte,const unsigned int TimeOut_ms=NULL);
    // Read a string (with timeout)
    int     ReadString  (   char *String,
                            char FinalChar,
                            unsigned int MaxNbBytes,
                            const unsigned int TimeOut_ms=NULL);
    // Read an array of byte (with timeout)
    int     Read        (void *Buffer,unsigned int MaxNbBytes,const unsigned int TimeOut_ms=NULL);

private:

    bool mDone;

    char mInterface[ INTERFACE_NAME_MAX_SIZE ];

    speed_t mSpeed;
    fd_set mFileDescriptorSet;
    // Read a string (no timeout)
    int ReadStringNoTimeOut  (char *String,char FinalChar,unsigned int MaxNbBytes);

    int fd;
    int32_t mFileDescriptor;

    std::mutex mMutex;

};



/*!  \class     TimeOut
     \brief     This class can manage a timer which is used as a timeout.
   */
// Class TimeOut
class TimeOut
{
public:

    // Constructor
    TimeOut();

    // Init the timer
    void                InitTimer();

    // Return the elapsed time since initialization
    unsigned long int   ElapsedTime_ms();

private:    
    struct timeval      PreviousTime;
};



/*!
  \mainpage Serial class

  \brief
       \htmlonly
       <TABLE>
       <TR><TD>
            <a href="../Serialv1.2.zip" title="Download the Serial class">
                <TABLE>
                <TR><TD><IMG SRC="download.png" BORDER=0 WIDTH=100> </TD></TR>
                <TR><TD><P ALIGN="center">[Download]</P> </TD></TR>
                </TABLE>
            </A>
            </TD>
            <TD>
                <script type="text/javascript"><!--google_ad_client = "ca-pub-0665655683291467";
                google_ad_slot = "0230365165";
                google_ad_width = 728;
                google_ad_height = 90;
                //-->
                </script>
                <script type="text/javascript"
                src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
                </script>
            </TD>
        </TR>
        </TABLE>

        \endhtmlonly

    The class Serial offers simple access to the serial port devices for windows and linux. It can be used for any serial device (Built-in serial port, USB to RS232 converter, arduino board or any hardware using or emulating a serial port)
    \image html Serial.png
    The class can be used under Windows and Linux.
    The class allows basic operations like :
    - opening and closing connection
    - reading data (characters, array of bytes or strings)
    - writing data (characters, array of bytes or strings)
    - non-blocking functions (based on timeout).


  \author   Philippe Lucidarme (University of Angers) <Serial@googlegroups.com>
  \date     1th may 2011 (Last update: 25th september 2012)
  \version  1.2

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is a licence-free software, it can be used by anyone who try to build a better world.
*/




#endif // SERIALIB_H

