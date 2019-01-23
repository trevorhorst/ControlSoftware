#include "beagleboneblack/hardware_beagleboneblack.h"

/**
 * @brief Constructor
 */
BeagleboneBlack::BeagleboneBlack()
    : Hardware()
    , mIndexHtml( Resources::load( Resources::INDEX_HTML, Resources::INDEX_HTML_SIZE ) )
    , mMainJs( Resources::load( Resources::MAIN_JS, Resources::MAIN_JS_SIZE ) )
    , mServer( mIndexHtml, mMainJs )
{
    // Add the individual commands
    addCommand( &mCmdSystem );
    addCommand( &mCmdDateTime );
    addCommand( &mCmdServer );

    // Set the command handler and start the server
    mServer.setCommandHandler( getCommandHandler() );
    mServer.listen();
}

/**
 * @brief Destructor
 */
BeagleboneBlack::~BeagleboneBlack()
{
    mServer.stop();
    Resources::unload( mIndexHtml );
    Resources::unload( mMainJs );
}
