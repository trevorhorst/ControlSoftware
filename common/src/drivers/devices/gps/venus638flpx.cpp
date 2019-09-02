#include "common/drivers/devices/gps/venus638flpx.h"

const uint8_t Venus638FLPx::Message::start_sequence[] = { 0xA0, 0xA1 };
const uint8_t Venus638FLPx::Message::end_sequence[] = { 0x0D, 0x0A };

const uint16_t Venus638FLPx::Message::start_sequence_length = sizeof( start_sequence );
const uint16_t Venus638FLPx::Message::end_sequence_length = sizeof( end_sequence );
const uint16_t Venus638FLPx::Message::payload_length = sizeof( uint16_t );
const uint16_t Venus638FLPx::Message::message_id_length = sizeof( uint8_t );
const uint16_t Venus638FLPx::Message::checksum_length = sizeof( uint8_t );

/**
 * @brief Constructor
 */
Venus638FLPx::Venus638FLPx( Serial *serial )
    : mSerial( serial )
{
    uint8_t msgBody[] = { 0x0 };
    Message newMsg( 0x2, msgBody, 1 );
    uint8_t *msg = reinterpret_cast< uint8_t* >( &newMsg );
    // for( size_t i = 0; i < ( sizeof( Message ) / sizeof( uint8_t ) ); i++ ) {
    //     printf( "%02x ", msg[ i ] );
    // }
}

int32_t Venus638FLPx::formMessage(
        int8_t messageId
        , const int8_t *messageBody
        , const int16_t messageBodySize
        , int8_t *message
        , int16_t *messageSize )
{
    int32_t error = 0;

    // *messageSize = start_sequence_length
    //         + end_sequence_length
    //         + payload_length
    //         + message_id_length
    //         + checksum_length
    //         + messageBodySize;

    // // Create a new message
    // message = new int8_t[ *messageSize ]();

    // memcpy( message, start_sequence, start_sequence_length );

    return error;
}
