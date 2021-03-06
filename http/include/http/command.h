#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include "common/command/command_template.h"
#include "http/server/server.h"

#define COMMAND_SERVER  "server"
#define COMMAND_QSERVER "qserver"

#define PARAM_ACTIVE    "active"
#define PARAM_PORT      "port"
#define PARAM_SECURE    "secure"

class CommandServer
        : public CommandTemplate< Http::Server >
{
public:
    CommandServer();
    virtual uint32_t getPort( cJSON *response );
    virtual uint32_t getActive( cJSON *response );
    virtual uint32_t getSecure( cJSON *response );
};

#endif // COMMAND_SERVER_H
