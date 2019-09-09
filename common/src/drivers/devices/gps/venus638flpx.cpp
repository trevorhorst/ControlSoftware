/** ***************************************************************************
 * @file venus638flpx.cpp
 * @author Trevor Horst
 * @copyright None
 * @brief Implementation of the Venus638FLPx driver
 * ****************************************************************************/

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

/**
 * @brief Retreive a pointer to the data
 * @return uint8_t*
 */
const uint8_t *Venus638FLPx::Message::getData()
{
    return mData;
}

/**
 * @brief Length of the data in the message
 * @return uint32_t
 */
uint32_t Venus638FLPx::Message::getDataLength()
{
    return mDataLength;
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
    setBaud( Serial::Speed::BAUD_9600 );
    // The device doesn't seem to acknowledge the following command if we don't
    // sleep some
    usleep( 25000 );
    dumpVersion();
}

/**
 * @brief Send a message to the device
 * @param message Message container
 * @param response Buffer to store the response
 * @param size Size of the response buffer
 * @return int32_t error code
 */
int32_t Venus638FLPx::sendMessage( Message &message, uint8_t *response, int32_t size )
{
    int32_t error = 0;

    // Write the message to the device
    error = mSerial->writeBytes( message.getData(), message.getDataLength() );

    // Read the response to our command
    if( error >= 0 ) {
        error = mSerial->readPattern(
                    Message::start_sequence, Message::start_sequence_length
                    , Message::end_sequence, Message::end_sequence_length
                    , response, size );
    }

    int32_t messageIdOffset = Message::start_sequence_length 
        + Message::end_sequence_length;
    
    // In the event the command was successful we will receive an ACK
    if( ( error >= 0 ) && ( response[ messageIdOffset ] == Message::Id::ACK ) ) {
        LOG_INFO( "ACK Received" );
        // We received an ACK, read again to get the verification of our command
        error = mSerial->readPattern(
                    Message::start_sequence, Message::start_sequence_length
                    , Message::end_sequence, Message::end_sequence_length
                    , response, size );
    }

    return error;
}

/**
 * @brief Send a message to the device
 * @param message Message container
 * @param response Buffer to store the response
 * @param size Size of the response buffer
 * @return int32_t error code
 */
int32_t Venus638FLPx::sendQuery( Message &message, uint8_t *response, int32_t size )
{
    int32_t error = 0;

    // Write the message to the device
    error = mSerial->writeBytes( message.getData(), message.getDataLength() );

    // Read the response to our command
    if( error >= 0 ) {
        error = mSerial->readPattern(
                    Message::start_sequence, Message::start_sequence_length
                    , Message::end_sequence, Message::end_sequence_length
                    , response, size );
    }

    int32_t messageIdOffset = Message::start_sequence_length
        + Message::end_sequence_length;

    // In the event the command was successful we will receive an ACK
    if( ( error >= 0 ) && ( response[ messageIdOffset ] == Message::Id::ACK ) ) {
        // We received an ACK, read again to get the verification of our command
        LOG_INFO( "ACK Received" );
        error = mSerial->readPattern(
                    Message::start_sequence, Message::start_sequence_length
                    , Message::end_sequence, Message::end_sequence_length
                    , response, size );
    }

    // The final response will contain the information we queried
    if( ( error >= 0 ) && ( response[ messageIdOffset ] == Message::Id::ACK ) ) {
        // We received a response for our message, read one more time
        LOG_INFO( "ACK Received" );
        error = mSerial->readPattern(
                    Message::start_sequence, Message::start_sequence_length
                    , Message::end_sequence, Message::end_sequence_length
                    , response, size );
    }

    return error;
}

uint32_t Venus638FLPx::getBaudRate()
{
    return mSerial->getInterfaceSpeed();
}

/**
 * @brief Sets the baud rate for the device
 * @param baud Desired baud rate
 * @return int32_t error code
 */
int32_t Venus638FLPx::setBaud( Serial::Speed baud )
{
    int32_t error = Error::Code::NONE;

    // Determine the baud setting for the device
    uint8_t baudToSet = 1;
    switch( baud ) {
        case Serial::Speed::BAUD_4800  : baudToSet = 0; break;
        case Serial::Speed::BAUD_9600  : baudToSet = 1; break;
        case Serial::Speed::BAUD_19200 : baudToSet = 2; break;
        case Serial::Speed::BAUD_38400 : baudToSet = 3; break;
        case Serial::Speed::BAUD_57600 : baudToSet = 4; break;
        case Serial::Speed::BAUD_115200: baudToSet = 5; break;
        default: error = Error::Code::PARAM_OUT_OF_RANGE;
    }

    uint8_t body[] = { 0x00, 0x00, 0x00 };
    body[ 1 ] = baudToSet;

    uint8_t buffer[ READ_BUFFER_SIZE ];
    Message message( Message::Id::CONFIGURE_SERIAL_PORT, body, sizeof( body ) );

    error = sendMessage( message, buffer, READ_BUFFER_SIZE );

    if( error >= 0 ) {
        mSerial->setInterfaceSpeed( baud );
    } else {
        LOG_ERROR( "failed to configure baud" );
    }

    return error;
}

/**
 * @brief Dumps version info from the device
 */
void Venus638FLPx::dumpVersion()
{
    int32_t error = 0;

    uint8_t body = 0;
    uint8_t buffer[ READ_BUFFER_SIZE ];

    Message message( Message::Id::QUERY_SOFTWARE_VERSION, &body, 1 );

    error = sendQuery( message, buffer, READ_BUFFER_SIZE );

    if( error >= 0 ) {
        int32_t idOffset = Message::start_sequence_length
                               + Message::end_sequence_length;

        uint8_t *kernel = &buffer[ idOffset + Message::SoftwareVersion::KERNEL_VERSION ];
        LOG_INFO( "Kernel Version: %02x.%02x.%02x"
            , kernel[ 1 ], kernel[ 2 ], kernel[ 3 ] );
        uint8_t *odm = &buffer[ idOffset + Message::SoftwareVersion::ODM_VERSION ];
        LOG_INFO( "   ODM Version: %02x.%02x.%02x"
            , odm[ 1 ], odm[ 2 ], odm[ 3 ] );
        uint8_t *revision = &buffer[ idOffset + Message::SoftwareVersion::REVISION ];
        LOG_INFO( "      Revision: %02x.%02x.%02x"
            , revision[ 1 ], revision[ 2 ], revision[ 3 ] );
    } else {
        LOG_ERROR( "failed to query version" );
    }
}

/**
 * @brief Prints the first GPS sentence found from the device
 */
void Venus638FLPx::printSentence()
{
    uint8_t buffer[ READ_BUFFER_SIZE ];
    mSerial->readPattern(
                reinterpret_cast< const uint8_t* >( gps_sentence_start_sequence )
                , strlen( gps_sentence_start_sequence )
                , reinterpret_cast< const uint8_t* >( gps_sentence_end_sequence )
                , strlen( gps_sentence_end_sequence )
                ,  buffer, READ_BUFFER_SIZE );
    buffer[ 255 ] = 0;
    LOG_INFO( "%s", buffer );
}
