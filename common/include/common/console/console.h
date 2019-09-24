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
#include "common/control/control_template.h"
#include "common/http/client/client.h"

#define CONSOLE_HISTORY_FILE            "control.history"
#define CONSOLE_HISTORY_FILE_LOCATION   "/tmp"

class Console
        : public ControlTemplate< Console >
        , public Singleton< Console >
{
    friend class Singleton< Console >;

public:
    void run();
    void quit();
    void evaluate( std::vector< std::string > input );
    static void processInput( char *input );
    static void evaluate( char *input );
    std::vector< std::string > tokenize( char *input, const char *delimiter = " " );

private:
    Console();
    ~Console();

    static Http::Client mClient;

    bool mDone;
};

#endif // CONSOLE_H
