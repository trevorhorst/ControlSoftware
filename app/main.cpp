#include <thread>
#include <iostream>

#include "common/console/console.h"
#include "common/command/command_console.h"
#include "common/command/command_help.h"

#include "hardware/hardware.h"

/**
 * @brief Entry point of the program
 * @return
 */
int main( int argc, char *argv[] )
{
    /// @todo Need to start handling arguments
    (void)argc;
    (void)argv;

    // Create the console
    Console *console = &Console::getInstance();

    // Create the hardware
    Hardware *hw = &Hardware::getInstance();

    CommandConsole cmdConsole;
    hw->addCommand( &cmdConsole );

    CommandHelp help;
    hw->addCommand( &help );

    std::thread *app = new std::thread( &Console::run, console );

    // Run the application and wait for its completion
    app->join();

    delete app;

    return 0;
}
