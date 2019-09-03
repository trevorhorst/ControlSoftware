#include "common/drivers/devices/gps/venus638flpx.h"

const uint8_t Venus638FLPx::Message::start_sequence[] = { 0xA0, 0xA1 };
const uint8_t Venus638FLPx::Message::end_sequence[] = { 0x0D, 0x0A };

const uint16_t Venus638FLPx::Message::start_sequence_length = sizeof( start_sequence );
const uint16_t Venus638FLPx::Message::end_sequence_length = sizeof( end_sequence );
const uint16_t Venus638FLPx::Message::payload_length = sizeof( uint16_t );
const uint16_t Venus638FLPx::Message::message_id_length = sizeof( uint8_t );
const uint16_t Venus638FLPx::Message::checksum_length = sizeof( uint8_t );

/**
 * @brief Message constructor
 * @param id Message ID
 * @param messageBody Message body
 * @param messageSize Size of the message body
 */
Venus638FLPx::Message::Message( uint8_t id, uint8_t *messageBody, uint16_t messageSize )
    : mData( nullptr )
    , mDataLength( start_sequence_length
                   + payload_length
                   + message_id_length
                   + messageSize
                   + checksum_length
                   + end_sequence_length )
{
    mData = new uint8_t[ mDataLength ]();

    uint32_t increment = 0;

    // Copy the start sequence
    memcpy( mData, start_sequence, start_sequence_length );
    increment += start_sequence_length;

    // Copy the payload
    uint16_t payloadLength = messageSize + message_id_length;
    mData[ increment + 0 ] = reinterpret_cast< uint8_t* >( &payloadLength )[ 1 ];
    mData[ increment + 1 ] = reinterpret_cast< uint8_t* >( &payloadLength )[ 0 ];
    increment += payload_length;

    // Copy the message id
    mData[ increment ] = id;
    increment += message_id_length;

    // Copy the message body
    memcpy( &mData[ increment ], messageBody, messageSize );
    increment += messageSize;

    // Create and copy the checksum
    uint8_t checksum = id;
    for( uint8_t i = 0; i < messageSize; i++ ) {
        checksum = checksum ^ messageBody[ i ];
    }
    mData[ increment ] = checksum;
    increment += checksum_length;

    // Copy the end sequence
    memcpy( &mData[ increment ], end_sequence, end_sequence_length );
    increment += end_sequence_length;

}

/**
 * @brief Message destrcutor
 */
Venus638FLPx::Message::~Message()
{
    if( mData ) {
        delete[] mData;
    }
}

const char Venus638FLPx::gpgga_sequence[] = "$GPGGA";
const char Venus638FLPx::gpgsa_sequence[] = "$GPGSA";
const char Venus638FLPx::gpgsv_sequence[] = "$GPGSV";
const char Venus638FLPx::gprmc_sequence[] = "$GPRMC";
const char Venus638FLPx::gpvtg_sequence[] = "$GPVTG";
const char Venus638FLPx::gps_sentence_start_sequence[] = "$";
const char Venus638FLPx::gps_sentence_end_sequence[] = "\n";

/**
 * @brief Constructor
 */
Venus638FLPx::Venus638FLPx( Serial *serial )
    : mSerial( serial )
{
    dumpSentences();
}

void Venus638FLPx::printSentence()
{
    char buffer[ 256 ];
    mSerial->readPattern( 
        gps_sentence_start_sequence, strlen( gps_sentence_start_sequence )
        , gps_sentence_end_sequence, strlen( gps_sentence_end_sequence )
        ,  buffer, 256 );
    buffer[ 255] = 0;
    printf( "%s", buffer );
}

void Venus638FLPx::dumpSentences()
{
    printSentence();
    printSentence();
    printSentence();
    printSentence();
    printSentence();
}