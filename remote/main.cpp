#include <thread>

#include "common/console/console.h"

int main( int argc, char *argv[] )
{
    (void)argc;
    (void)argv;

    Console *console = &Console::getInstance();

    std::thread *app = new std::thread( &Console::run, console );

    // Run the application and wait for its completion
    app->join();

    return 0;
}
