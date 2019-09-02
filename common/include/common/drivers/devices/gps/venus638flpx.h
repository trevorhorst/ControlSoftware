#ifndef VENUS638FLPX_H
#define VENUS638FLPX_H

#include "common/drivers/serial.h"

class Venus638FLPx 
{
    struct Message {
        uint8_t *mData;
        uint32_t mDataLength;

        static const uint8_t start_sequence[];
        static const uint8_t end_sequence[];
        static const uint16_t payload_length;
        static const uint16_t message_id_length;
        static const uint16_t checksum_length;
        static const uint16_t start_sequence_length;
        static const uint16_t end_sequence_length;

        /**
         * @brief Message constructor
         * @param id Message ID
         * @param messageBody Message body
         * @param messageSize Size of the message body
         */
        Message( uint8_t id, uint8_t *messageBody, uint16_t messageSize )
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
        ~Message()
        {
            if( mData ) {
                delete[] mData;
            }
        }
    };

public:
    Venus638FLPx( Serial *serial );

    int32_t formMessage( int8_t messageId
                         , const int8_t *messageBody
                         , const int16_t messageBodySize
                         , int8_t *message
                         , int16_t *messageSize );

private:

    static const char start_sequence[];
    static const char end_sequence[];

    static const int8_t start_sequence_length;
    static const int8_t end_sequence_length;
    static const int8_t payload_length;
    static const int8_t message_id_length;
    static const int8_t checksum_length;

    Serial *mSerial;
};

#endif // VENUS638FLPX_H
