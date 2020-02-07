#ifndef COMMAND_SMTP_H
#define COMMAND_SMTP_H

#include "common/command/command_template.h"
#include "common/smtp/client/client.h"

#define COMMAND_SMTP    "smtp"
#define COMMAND_QSMTP   "qsmtp"

#define PARAM_PASSWORD  "password"
#define PARAM_USERNAME  "username"
#define PARAM_SEND      "send"
#define PARAM_SERVER    "server"

class CommandSmtp
        : public CommandTemplate< Smtp::Client >
{
public:
    CommandSmtp();
    uint32_t getServer( cJSON *val );

    uint32_t setUsername( cJSON *val );
    uint32_t setPassword( cJSON *val );
    uint32_t setSend( cJSON *val );
};

#endif // COMMAND_SMTP_H
