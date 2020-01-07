#include <thread>
#include <iostream>

#include "common/option_parser.h"
#include "common/console/console.h"
#include "common/command/command_console.h"

#include "hardware/hardware.h"

enum OptionIndex {
    UNKNOWN  = 0
    , HELP
    , SIMULATED
    , NUM_ARGUMENTS
};

const option::Descriptor usage[] = {
    { UNKNOWN, 0, "" , "", option::Arg::None, "USAGE: example [options]\n\n"
                                               "Options:" },
    { HELP, 0, "h" , "help", option::Arg::None, "  --help  \tPrint usage and exit." },
    { SIMULATED, 0, "s" , "simulated", option::Arg::None, "  --simulated  \tSimulate hardware." },
    { 0, 0, nullptr, nullptr, nullptr, nullptr }
};

/**
 * @brief Entry point of the program
 * @return
 */
int main( int argc, char *argv[] )
{
    // Skip program name if present
    argc -= ( argc > 0 );
    argv += ( argc > 0 );

    option::Stats  stats(usage, argc, argv);
    std::vector<option::Option> options(stats.options_max);
    std::vector<option::Option> buffer(stats.buffer_max);
    option::Parser parse(usage, argc, argv, &options[0], &buffer[0]);

    if( parse.error() ) {
        return 1;
    }

    if( options[ HELP ] ) {
      option::printUsage( std::cout, usage );
      return 0;
    }

    bool simulated = false;
    if( options[ SIMULATED ] ) {
        simulated = true;
    }

    // Create the console
    Console *console = &Console::getInstance();

    /// @todo Need a more robust way to create hardware so that we can configure
    /// more options at runtime
    // Create the hardware
    Hardware::setSimulated( simulated );
    Hardware *hw = &Hardware::getInstance();

    CommandConsole cmdConsole;
    hw->addCommand( &cmdConsole );

    std::thread *app = new std::thread( &Console::run, console );

    // Run the application and wait for its completion
    app->join();

    delete app;

    return 0;
}
