/** ***************************************************************************
 * @file        console.cpp
 * @author      Trevor Horst
 * @copyright   G3 Technologies, Inc. 2018
 * @brief       Console class definitions
 *
 * Provides a command line interface for command and control functionality.
 * Incorproates use of the GNU Readline library.
 * ****************************************************************************/
#include "common/console/console.h"

#define CONSOLE_MAX_TOKENS  10
#define CONSOLE_MAX_TOKEN_SIZE 64

/**
 * @brief Constructor
 */
Console::Console()
    : mDone( false )
{
}

/**
 * @brief Destructor
 */
Console::~Console()
{
    CommandMap::const_iterator it;
    for( it = mCommandMap.begin(); it != mCommandMap.end(); it++ ) {
        delete it->second;
    }
}

#define MAX_HISTORY_LINES 1000
#define HISTORY_FNAME "control.history"

/**
 * @brief Console::Run Main thread loop
 */
void Console::run()
{
    printf( "Build Time: %s %s\n", __DATE__, __TIME__ );

    // Retrieve the current terminal attributes
    struct termios term;
    if( tcgetattr( STDIN_FILENO, &term ) < 0 ) {
        perror( "tcgetattr" );
        exit( 1 );
    }

    // Save off current settings
    tcflag_t old_lflag = term.c_lflag;
    cc_t old_vtime = term.c_cc[ VTIME ];
    // AND out the ICANON flag - Place the terminal in non canonical mode
    term.c_lflag &= ~ICANON;
    term.c_cc[ VTIME ] = 1;
    if( tcsetattr( STDIN_FILENO, TCSANOW, &term ) < 0 ) {
        perror("tcsetattr");
        exit(1);
    }

    char hfile[ 512 ];
    snprintf( hfile, sizeof( hfile ), "%s/%s"
              , CONSOLE_HISTORY_FILE_LOCATION, CONSOLE_HISTORY_FILE );
    if( read_history( hfile ) ) {
        perror("read_history");
    }
     
    // Set the readline callback handler
    rl_callback_handler_install( ">", &Console::evaluate );

    fd_set fds;
    while( !mDone ) {
        // Set the timeout
        struct timeval tv;
        tv.tv_sec=0;
        tv.tv_usec=50000;
        // Zeroize file descriptors
        FD_ZERO( &fds );
        // Set the file descriptor to the standard input
        FD_SET( fileno( stdin ), &fds );

        /// @note select() and pselect() allow a program to monitor multiple file
        /// descriptors, waiting until one or more of the file descriptors become
        /// "ready" for some class of I/O operation (e.g., input possible).
        if( select( FD_SETSIZE, &fds, NULL, NULL, &tv ) < 0 ) {
            perror("select");
            break;
        }

        if( FD_ISSET( fileno( stdin ), &fds ) ) {
            // File descriptor is ready, read characters
            rl_callback_read_char();
        } else {
            // If we don't find any new data
        }
    }
    
    if( write_history( hfile ) ) {
        perror( "write_history" );
    } else {
        // Keep the history to minimum.
        history_truncate_file( hfile, MAX_HISTORY_LINES );
    }

    // Restore previous flag settings
    term.c_lflag     = old_lflag;
    term.c_cc[VTIME] = old_vtime;
    // Set the terminal attributes
    if( tcsetattr(STDIN_FILENO, TCSANOW, &term) < 0 ) {
         perror("tcsetattr");
         exit(1);
    }

    rl_callback_handler_remove();

    printf("\nExitting...\n");
}

void Console::quit()
{
    mDone = true;
}

/**
 * @brief Console::processLine Wrapper function to evaluate a line
 * @param line Line to evaluate
 */
void Console::processInput( char *input )
{
    std::vector< std::string > tokenized = getInstance().tokenize( input );
    getInstance().evaluate( tokenized );
    if( input ) {
        add_history( input );
    }
    free( input );
}

std::vector<std::string> Console::tokenize( char *input, const char *delimiter )
{
    std::string str( input );
    std::string token;
    std::vector< std::string > tokenized;
    size_t last = 0;
    size_t next = 0;

    // Split the input into segments by the delimeter
    while( ( next = str.find( delimiter, last ) ) != std::string::npos ) {
        if( next - last != 0 ) {
            // There is data in the string
            token = str.substr( last, next - last );
            tokenized.push_back( token );
        } else {
            // If the next and last values are the same, the string is empty
            // and we can ignore it
        }
        last = next + 1;
    }


    if( !str.substr( last ).empty() ) {
        // Need to place the the final substring in the list, unless it's empty
        tokenized.push_back( str.substr( last ) );
    }

    return tokenized;
}

void Console::evaluate( char *input )
{
    // Tokenize the input
    std::vector< std::string > tokenized = getInstance().tokenize( input );

    if( tokenized.empty() ) {
        // The input is empty
        return;
    }

    if( tokenized.at( 0 ) == "quit" ) {
        // The command is to quit, so lets quit. Nothing fancy here.
        getInstance().quit();
        return;
    }

    // Add the cmd string to the object
    cJSON *msg    = cJSON_CreateObject();
    cJSON *params = cJSON_CreateObject();
    cJSON *cmd    = nullptr;

    for( auto it = tokenized.begin(); it != tokenized.end(); it++ ) {
        if( *it == tokenized.at( 0 ) ) {
            // Add the command parameter
            cmd = cJSON_CreateString( (*it).c_str() );
        } else {
            auto t = it;
            it++;
            if( it == tokenized.end() ) {
                printf( "Parameter mismatch\n" );
            } else {
                // Parse the parameter
                cJSON *param = cJSON_Parse( (*it).c_str() );
                // Add the item to the parameter list
                cJSON_AddItemToObject( params, (*t).c_str(), param );
            }
        }
    }

    cJSON_AddItemToObject( msg, PARAM_COMMAND, cmd );
    cJSON_AddItemToObject( msg, PARAM_PARAMS, params );

    char *msgStr = cJSON_PrintUnformatted( msg );

    if( getInstance().isVerbose() ) {
        printf( "%s\n", msgStr );
    }

    HttpClient client;
    client.send( msgStr );
    cJSON_free( msgStr );

    // Clean up the message, this should delete all the components
    cJSON_Delete( msg );

    if( input ) {
        add_history( input );
    }

    free( input );
}

void Console::evaluate( std::vector<std::string> input )
{
    if( input.empty() ) {
        // The input is empty, nothing to do
        return;
    }

    if( input.at( 0 ) == "quit" ) {
        getInstance().quit();
        return;
    }

    // Add the cmd string to the object
    cJSON *msg    = cJSON_CreateObject();
    cJSON *params = cJSON_CreateObject();
    cJSON *cmd    = nullptr;

    for( auto it = input.begin(); it != input.end(); it++ ) {
        if( *it == input.at( 0 ) ) {
            // Add the command parameter
            cmd = cJSON_CreateString( (*it).c_str() );
        } else {
            auto t = it;
            it++;
            if( it == input.end() ) {
                printf( "Parameter mismatch\n" );
                break;
            } else {
                // Parse the parameter
                cJSON *param = cJSON_Parse( (*it).c_str() );
                // Add the item to the parameter list
                cJSON_AddItemToObject( params, (*t).c_str(), param );
            }
        }
    }

    cJSON_AddItemToObject( msg, PARAM_COMMAND, cmd );
    cJSON_AddItemToObject( msg, PARAM_PARAMS, params );

    char *msgStr = cJSON_PrintUnformatted( msg );

    if( getInstance().isVerbose() ) {
        printf( "%s\n", msgStr );
    }

    HttpClient client;
    client.send( msgStr );
    cJSON_free( msgStr );

    // Clean up the message, this should delete all the components
    cJSON_Delete( msg );

    /*
    CommandMap::const_iterator it = mCommandMap.find( input.at( 0 ).c_str() );
    if( it == mCommandMap.end() ) {
        // A matching command was not found
        if( input.at( 0 ) == "quit" ) {
            getInstance().quit();
        }
        return;
    }

    cJSON *params = cJSON_CreateObject();
    CommandContainer *cmd = it->second;

    for( size_t s = 1; s < input.size(); s += 2 ) {
        std::string key = input.at( s );
        if( key.length() == 0 ) {
            printf( "Key is empty\n " );
            return;
        }

        if( input.at( s ) == "help"
                || s + 1 >= input.size() ) {
            // Improper use of the command or asking for help
            printf( "%s\n", cmd->mCmdObj->usage() );
            return;
        }

        cJSON_AddItemToObject( params
                               , key.c_str()
                               , cJSON_Parse( input.at( s + 1 ).c_str() ) );

    }

    cJSON *rsp = cmd->call( params );
    char *rspText = cJSON_Print( rsp );
    printf( "%s\n", rspText );
    free( rspText );

    cJSON_Delete( rsp );
    cJSON_Delete( params );
    */
}

void Console::addCommand( Command *cmd )
{
    if( cmd->isAccessible() ) {
        // The command is accessible
        mCommandMap[ cmd->getAccessorName() ]
                = new CommandContainer( Command::Type::ACCESSOR, cmd );
    }

    if( cmd->isMutable() ) {
        // The command is mutable
        mCommandMap[ cmd->getMutatorName() ]
                = new CommandContainer( Command::Type::MUTATOR, cmd );
    }
}
