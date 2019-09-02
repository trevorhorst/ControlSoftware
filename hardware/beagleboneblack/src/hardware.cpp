#include "hardware/hardware.h"

const char *Hardware::str_gps_device = "/dev/ttyUSB0";

/**
 * @brief Constructor
 */
Hardware::Hardware()
    : HardwareBase()
    , mIndexHtml( Resources::load( Resources::index_html, Resources::index_html_size ) )
    , mBundleJs( Resources::load( Resources::bundle_js, Resources::bundle_js_size ) )
    , mGpsSerial( str_gps_device, Serial::Speed::BAUD_9600 )
    , mGps( &mGpsSerial )
    , mControlModule( AM335X::addr_control_module, isSimulated() )
    , mClockModule( AM335X::addr_clock_module, isSimulated() )
    , mGpio{ { AM335X::addr_gpio0_base, isSimulated() }
             , { AM335X::addr_gpio1_base, isSimulated() }
             , { AM335X::addr_gpio2_base, isSimulated() }
             , { AM335X::addr_gpio3_base, isSimulated() }
             }
    , mServer( mIndexHtml, mBundleJs )
    , mSerialMonitor( nullptr )
{
    // Add the individual commands
    addCommand( &mCmdGpio );
    addCommand( &mCmdSystem );
    addCommand( &mCmdDateTime );
    addCommand( &mCmdServer );
    addCommand( &mCmdLed );

    // Set the command handler and start the server
    mServer.setCommandHandler( getCommandHandler() );
    mServer.listen();

    // mSerialMonitor = new std::thread( &Serial::readInterface, &mGpsSerial );
    // mGpsSerial.readInterface();
    // mGpsSerial.Read( str, 1000, 2000 );
    // printf( "String Read: '%s'\n", str );
}

/**
 * @brief Destructor
 */
Hardware::~Hardware()
{
    mGpsSerial.setDone();

    if( mSerialMonitor ) {
        mSerialMonitor->join();
        delete mSerialMonitor;
        mSerialMonitor = nullptr;
    }

    mServer.stop();
    Resources::unload( mIndexHtml );
    Resources::unload( mBundleJs );
}
