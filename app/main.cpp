#include <thread>
#include <iostream>

#include "common/console/console.h"
#include "common/command/command.h"
#include "common/http/server/server.h"
#include "common/http/client/client.h"
#include "common/system/datetime.h"
#include "beagleboneblack/hardware_beagleboneblack.h"

#include "common/command/command_console.h"
#include "common/command/command_server.h"
#include "common/command/command_system.h"
#include "common/command/command_datetime.h"
#include "common/command/command_help.h"

/**
 * @brief Entry point of the program
 * @return
 */
int main()
{
    // Create the console
    Console *console = &Console::getInstance();

    // Create the hardware
    BeagleboneBlack *bbb = &BeagleboneBlack::getInstance();

    CommandConsole cmdConsole;
    bbb->addCommand( &cmdConsole );

    CommandHelp help;
    bbb->addCommand( &help );

    std::thread *app = new std::thread( &Console::run, console );
    // std::thread *req = new std::thread( &HttpServer::handleRequests, &server );
    // req->detach();
    app->join();

    delete app;

    return 0;
}
