#include <thread>
#include <iostream>

#include "common/console/console.h"
#include "common/command/command.h"
#include "common/http/server/server.h"
#include "common/http/client/client.h"
#include "beagleboneblack/hardware_beagleboneblack.h"

#include "common/command/command_console.h"

/**
 * @brief Entry point of the program
 * @return
 */
int main()
{
    CommandHandler commandHandler;

    Control ctl;
    Command cmd( "tuner", "qtuner" );
    cmd.addControlObject( &ctl );
    // commandHandler.addCommand( &cmd );

    CommandConsole cmdConsole;
    commandHandler.addCommand( &cmdConsole );

    // printf( "%s\n", cmd.getAccessorName() );

    // bindCommand( &cmd );

    // cJSON *p = cJSON_CreateObject();
    // cJSON_AddBoolToObject( p, PARAM_VERBOSE, true );

    // // Need to make sure we free our cJSON print strings
    // char *str = cJSON_Print( p );
    // printf( "%s\n", str );
    // free( str );

    // CommandCallback cb = mCommandMap[ COMMAND_QTUNER ];
    // cJSON *response = cb( p );
    // str = cJSON_Print( response );
    // printf( "Response: %s\n", str );
    // free( str );
    // cJSON_Delete( response );

    // cJSON_Delete( p );
    BeagleboneBlack *bbb = &BeagleboneBlack::getInstance();
    (void)bbb;


    HttpServer server( Resources::INDEX_HTML
                       , Resources::MAIN_JS );
    server.setCommandHandler( &commandHandler );
    server.listen();

    std::thread *app = new std::thread( &Console::run, &Console::getInstance() );
    // std::thread *req = new std::thread( &HttpServer::handleRequests, &server );
    // req->detach();
    app->join();

    server.mDone = true;

    server.stop();

    delete app;

    return 0;
}
