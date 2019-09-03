#ifndef VENUS638FLPX_H
#define VENUS638FLPX_H

#include <stdint.h>

#include "common/drivers/serial.h"

class Venus638FLPx 
{
    struct Message {

        enum Id {
            NONE                        = 0x00
            , QUERY_SOFTWARE_VERSION    = 0x02
            , SOFTWARE_VERSION          = 0x81
            , SOFTWARE_CRC              = 0x82
            , ACK                       = 0x83
            , NACK                      = 0x84
        };

        enum SoftwareVersion {
            MESSAGE_ID          = 0x00
            , SOFTWARE_TYPE     = 0x01
            , KERNEL_VERSION    = 0x02
            , ODM_VERSION       = 0x06
            , REVISION          = 0x0A
        };

        uint8_t *mData;
        uint32_t mDataLength;

        static const uint8_t start_sequence[];
        static const uint8_t end_sequence[];
        static const uint16_t payload_length;
        static const uint16_t message_id_length;
        static const uint16_t checksum_length;
        static const uint16_t start_sequence_length;
        static const uint16_t end_sequence_length;

        Message( uint8_t id, uint8_t *messageBody, uint16_t messageSize );
        ~Message();

        const uint8_t *getData();
        uint32_t getDataLength();
    };

public:
    Venus638FLPx( Serial *serial );

    int32_t sendMessage( Message &message, uint8_t *response, int32_t size );

    void dumpVersion();
    void printSentence();

private:

    static const char start_sequence[];
    static const char end_sequence[];

    static const char gpgga_sequence[];
    static const char gpgsa_sequence[];
    static const char gpgsv_sequence[];
    static const char gprmc_sequence[];
    static const char gpvtg_sequence[];
    static const char gps_sentence_start_sequence[];
    static const char gps_sentence_end_sequence[];

    static const int8_t start_sequence_length;
    static const int8_t end_sequence_length;
    static const int8_t payload_length;
    static const int8_t message_id_length;
    static const int8_t checksum_length;

    Serial *mSerial;
};

#endif // VENUS638FLPX_H
