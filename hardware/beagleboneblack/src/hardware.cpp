#include "hardware/hardware.h"

/**
 * @brief Constructor
 */
Hardware::Hardware()
    : HardwareBase()
    , mIndexHtml( Resources::load( Resources::index_html, Resources::index_html_size ) )
    , mBundleJs( Resources::load( Resources::bundle_js, Resources::bundle_js_size ) )
    , mClockModule{ AM335X::addr_clock_module, isSimulated() }
    , mGpio{ { AM335X::addr_gpio0_base, isSimulated() }
             , { AM335X::addr_gpio1_base, isSimulated() }
             , { AM335X::addr_gpio2_base, isSimulated() }
             , { AM335X::addr_gpio3_base, isSimulated() }
             }
    , mServer( mIndexHtml, mBundleJs )
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
}

/**
 * @brief Destructor
 */
Hardware::~Hardware()
{
    mServer.stop();
    Resources::unload( mIndexHtml );
    Resources::unload( mBundleJs );
}
