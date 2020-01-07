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

class Console
        : public ControlTemplate< Console >
        , public Singleton< Console >
{
    friend class Singleton< Console >;

    static const char *history_file;
    static const char *history_file_location;

    static Http::Client client;

public:
    void run();
    void quit();
    static void evaluate( char *input );
    std::vector< std::string > tokenize( char *input, const char *delimiter = " " );

private:
    Console();
    ~Console();


    bool mDone;
};

#endif // CONSOLE_H
