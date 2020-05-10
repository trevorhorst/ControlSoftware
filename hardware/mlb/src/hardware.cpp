#include "hardware/hardware.h"

const uint32_t Hardware::heartbeat_delay_1000_ms = 1000;

const char *Hardware::mlb_api_host = "http://lookup-service-prod.mlb.com";
const char *Hardware::mlb_api_path = "/json/named.%s.bam?";
const char *Hardware::endpoint_transaction_all  = "transaction_all";
const char *Hardware::endpoint_wsfb_news_injury = "wsfb_news_injury";

const char *Hardware::mlb_api =
        "http://lookup-service-prod.mlb.com/json/named.transaction_all.bam?sport_code='mlb'&start_date='%s'&end_date='%s'";
const char *Hardware::smtp_gmail_server = "smtp://smtp.gmail.com:587";

const char *Hardware::email_list[256] = {
    "trevorhorst1212@gmail.com"
    , "trevor.w.horst@gmail.com"
};

const char *Hardware::mlb_api_date = "%Y%m%d";

/**
 * @brief Constructor
 */
Hardware::Hardware()
    : HardwareBase()
    , mIndexHtml( Resources::load( Resources::index_html, Resources::index_html_size ) )
    , mBundleJs( Resources::load( Resources::bundle_js, Resources::bundle_js_size ) )
    , mServer( mIndexHtml, mBundleJs )
    , mHeartbeatTimer( heartbeat_delay_1000_ms
                       , Timer::Type::INTERVAL
                       , std::bind( &Hardware::heartbeat, this ) )
{
    // Configure the SMTP client
    // mSmtpClient.setUsername( "Put username here" );
    // mSmtpClient.setPassword( "Put password here" );
    // mSmtpClient.addTo( "Add recipient here" );
    mSmtpClient.setServer( smtp_gmail_server );
    mSmtpClient.setReadFunction( &Smtp::Client::readFunction );
    mSmtpClient.setSubject( "MLB Transactions" );
    mSmtpClient.applySettings();


    // Add the individual commands
    addCommand( &mCmdHelp );
    addCommand( &mCmdHeartbeat );
    addCommand( &mCmdSystem );
    addCommand( &mCmdDateTime );
    addCommand( &mCmdServer );
    addCommand( &mCmdSmtp );

    // Set the command handler and start the server
    mServer.setCommandHandler( getCommandHandler() );
    mServer.listen();

    queryApi();

    mHeartbeatTimer.start();
}

/**
 * @brief Destructor
 */
Hardware::~Hardware()
{
    // Destruct things in the reverse order
    mHeartbeatTimer.stop();

    mServer.stop();
    Resources::unload( mIndexHtml );
    Resources::unload( mBundleJs );
}

/**
 * @brief Heartbeat for the hardware, update pertinant hardware information from
 * this method
 */
void Hardware::heartbeat()
{
}

uint32_t Hardware::getDate( char *buffer, size_t size )
{
    uint32_t error = Error::Code::NONE;

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime( &rawtime );

    strftime( buffer, size, mlb_api_date, timeinfo );

    return error;
}

uint32_t Hardware::queryApi()
{
    // char date[ 9 ];
    // getDate( date, 19);
    // char getUrl[512];
    // snprintf( getUrl, 256, mlb_api, date, date );
    // mHttpClient.applyUrl( getUrl );

    // // Parse the information retrieved from the site
    // cJSON *parsed = cJSON_Parse( mHttpClient.get().c_str() );
    // cJSON *transactionAll = cJSON_GetObjectItem( parsed, "transaction_all" );
    // cJSON *queryResults = cJSON_GetObjectItem( transactionAll, "queryResults" );
    // cJSON *totalSize    = cJSON_GetObjectItem( queryResults, "totalSize" );
    // if( cJSON_IsString( totalSize ) ) {
    //     int32_t size = atoi( totalSize->valuestring );
    //     printf( "TotalSize: %d\n",  size );
    //     cJSON *row = cJSON_GetObjectItem( queryResults, "row" );
    //     if( cJSON_IsArray( row ) ) {
    //         for( int32_t i = 0; i < size; i++ ) {
    //             cJSON *item = cJSON_GetArrayItem( row, i );
    //             char *printItem = cJSON_Print( item );
    //             printf( "%s\n", printItem );
    //             free( printItem );
    //         }
    //     }
    // }
    // cJSON_Delete( parsed );
    return 0;
}
