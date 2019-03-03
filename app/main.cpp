#include <thread>
#include <iostream>

#include "common/console/console.h"
#include "beagleboneblack/hardware_beagleboneblack.h"

#include "common/command/command_console.h"
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

    // Run the application and wait for its completion
    app->join();

    delete app;

    return 0;
}
