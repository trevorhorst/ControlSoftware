/** ****************************************************************************
  * @file       console.h
  * @author     Trevor Horst
  * @copyright  G3 Technologies, Inc. 2018
  * @brief      Console class declarations
  * ***************************************************************************/
#ifndef CONSOLE_H
#define CONSOLE_H

#include <sys/types.h>
#include <termio.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>

#include "common/singleton.h"
#include "common/command/command.h"
#include "common/http/client/client.h"

class Console
        : public Singleton< Console >
        , public Control
{
    friend class Singleton< Console >;

    struct CommandContainer {
        CommandContainer( Command::Type type, Command *cmdObj )
            : mType( type )
            , mCmdObj( cmdObj )
        {}
        Command::Type mType;
        Command* mCmdObj;
        cJSON *call( cJSON *params ) {
            cJSON *rsp = NULL;
            if( mType == Command::ACCESSOR ) {
                rsp = mCmdObj->access( params );
            } else if( mType == Command::MUTATOR ) {
                rsp = mCmdObj->mutate( params );
            }
            return rsp;
        }
    };

    using CommandMap = CharHashMap< CommandContainer* >;

public:
    void run();
    void quit();
    static void processInput( char *input );
    std::vector< std::string > tokenize( char *input, const char *delimiter = " " );
    void evaluate( std::vector< std::string > input );

    void addCommand( Command *cmd );

private:
    Console();
    ~Console();

    CommandMap mCommandMap;

    bool mDone;
};

// class CommandConsole : public Command {
//     using Callback = cJSON* (Console::*)( cJSON *params );
// public:
//     CommandConsole( const char *mutator )
//         : Command( mutator, "" )
//     {
//         bindMutatorCallback( "quit", &CommandConsole::setQuit );
//     }
//
//     uint32_t setQuit( cJSON *val )
//     {
//         uint32_t r = Error::Code::NONE;
//         (void)val;
//         Console::getInstance().quit();
//         return r;
//     }
//
// private:
//
// };

#endif // CONSOLE_H
