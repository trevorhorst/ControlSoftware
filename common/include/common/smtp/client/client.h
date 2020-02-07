#ifndef SMTP_CLIENT_H
#define SMTP_CLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h>

#include "common/control/control_template.h"

namespace Smtp
{

extern const char *smtp_gmail_server;

class Client
        : public ControlTemplate< Client >
{
    using ReadFunction = size_t ( void*, size_t, size_t, void* );

    struct UploadStatus
    {
        int32_t linesRead;
    };

    struct Payload
    {
        uint32_t sent;
        std::string date;
        std::string to;
        std::string cc;
        std::string subject;
        std::string message;
    };

    struct Settings
    {
        uint32_t mask;
        std::vector< std::string > toList;
        std::vector< std::string > carbonCopyList;
        std::string server;
        std::string username;
        std::string password;
        std::string subject;
        std::string message;
        ReadFunction *readFunction;
    };

    static uint32_t set_counter;
    static const uint32_t set_server;
    static const uint32_t set_username;
    static const uint32_t set_password;
    static const uint32_t set_subject;
    static const uint32_t set_recipients;
    static const uint32_t set_read_function;

    static const char *gmail_smtp_server;

public:
    Client();
    ~Client() override;

    void close();

    std::string getServer();

    uint32_t send( const std::string &message );

    uint32_t addTo( const std::string recipient );
    uint32_t addCarbonCopy( const std::string recipient );

    uint32_t setServer( const std::string &server );
    uint32_t setUsername( const std::string &username );
    uint32_t setPassword( const std::string &password );
    uint32_t setSubject( const std::string &subject );
    uint32_t setReadFunction( ReadFunction *readFunction );

    uint32_t applyDate();
    uint32_t applyServer( const std::string &server );
    uint32_t applyUsername( const std::string &username );
    uint32_t applyPassword( const std::string &password );
    uint32_t applyRecipients( const std::vector< std::string > &toList
                              , const std::vector< std::string > &ccList );
    uint32_t applySubject( const std::string &subject );
    uint32_t applyReadFunction( ReadFunction *readFunction );

    uint32_t applySettings() override;

private:
    CURL *mCurl;

    Payload mPayload;
    Settings mSettings;

    static ReadFunction readFunction;
    static ReadFunction newReadFunction;
};

}

#endif // SMTP_CLIENT_H
