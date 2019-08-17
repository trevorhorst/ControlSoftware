#include "hardware/hardware.h"

/**
 * @brief Constructor
 */
Hardware::Hardware()
    : HardwareBase()
    , mIndexHtml( Resources::load( Resources::INDEX_HTML, Resources::INDEX_HTML_SIZE ) )
    , mBundleJs( Resources::load( Resources::BUNDLE_JS, Resources::BUNDLE_JS_SIZE ) )
    , mGpio{ { AM335X::gpio0_base, isSimulated() }
             , { AM335X::gpio1_base, isSimulated() }
             , { AM335X::gpio2_base, isSimulated() }
             , { AM335X::gpio3_base, isSimulated() }
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
