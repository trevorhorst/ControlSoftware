#include "common/command/command_console.h"

CommandConsole::CommandConsole()
    : Command( COMMAND_CONSOLE, COMMAND_QCONSOLE )
{
    // Lets just go ahead and set the control object here
    mControlObject = &Console::getInstance();

    mMutatorMap[ PARAM_QUIT ] = PARAMETER_CALLBACK( &CommandConsole::setQuit );
}

uint32_t CommandConsole::setQuit( cJSON *val )
{
    (void)val;
    uint32_t r = Error::Code::NONE;
    if( cJSON_IsBool( val ) ) {
        if( cJSON_IsTrue( val ) ) {
            Console::getInstance().quit();
        }
    } else {
        r = Error::Code::PARAM_INVALID;
    }
    return r;
}
