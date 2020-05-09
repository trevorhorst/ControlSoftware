#include "hardware/hardware.h"

const uint32_t Hardware::heartbeat_delay_1000_ms = 1000;

const char *Hardware::smtp_gmail_server = "smtp://smtp.gmail.com:587";

/**
 * @brief Constructor
 */
Hardware::Hardware()
    : HardwareBase()
    , mIndexHtml( Resources::load( Resources::index_html, Resources::index_html_size ) )
    , mBundleJs( Resources::load( Resources::bundle_js, Resources::bundle_js_size ) )
    , mServer( mIndexHtml, mBundleJs )
    , mHeartbeatTimer( heartbeat_delay_1000_ms
                       , Timer::Type::INTERVAL
                       , std::bind( &Hardware::heartbeat, this ) )
{
    // mSmtpClient.setUsername( "Put username here" );
    // mSmtpClient.setPassword( "Put password here" );
    // mSmtpClient.addTo( "Add recipient here" );
    mSmtpClient.setServer( smtp_gmail_server );
    mSmtpClient.setReadFunction( &Smtp::Client::newReadFunction );
    mSmtpClient.setSubject( "ControlSoftware Example" );
    mSmtpClient.addTo( "trevorhorst1212@gmail.com" );
    mSmtpClient.applySettings();

    // Add the individual commands
    addCommand( &mCmdHelp );
    addCommand( &mCmdHeartbeat );
    addCommand( &mCmdSystem );
    addCommand( &mCmdDateTime );
    addCommand( &mCmdServer );
    addCommand( &mCmdSmtp );

    // Set the command handler and start the server
    mServer.setCommandHandler( getCommandHandler() );
    mServer.listen();

    mHeartbeatTimer.start();
}

/**
 * @brief Destructor
 */
Hardware::~Hardware()
{
    // Destruct things in the reverse order
    mHeartbeatTimer.stop();

    mServer.stop();
    Resources::unload( mIndexHtml );
    Resources::unload( mBundleJs );
}

/**
 * @brief Heartbeat for the hardware, update pertinant hardware information from
 * this method
 */
void Hardware::heartbeat()
{
}
