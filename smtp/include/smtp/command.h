#ifndef COMMAND_SMTP_H
#define COMMAND_SMTP_H

#include "common/command/command_template.h"
#include "smtp/client.h"

#define COMMAND_SMTP    "smtp"
#define COMMAND_QSMTP   "qsmtp"

#define PARAM_PASSWORD  "password"
#define PARAM_USERNAME  "username"
#define PARAM_SEND      "send"
#define PARAM_SERVER    "server"
#define PARAM_TO        "to"
#define PARAM_CC        "cc"
#define PARAM_RECIPIENTS "recipients"

class CommandSmtp
        : public CommandTemplate< Smtp::Client >
{
public:
    CommandSmtp();
    uint32_t getServer( cJSON *val );
    uint32_t getRecipients( cJSON *val );

    uint32_t setTo( cJSON *val );
    uint32_t setCc( cJSON *val );
    uint32_t setServer( cJSON *val );
    uint32_t setUsername( cJSON *val );
    uint32_t setPassword( cJSON *val );
    uint32_t setSend( cJSON *val );
};

#endif // COMMAND_SMTP_H
