#ifndef SSD1306_H
#define SSD1306_H

#include "common/drivers/i2c.h"
#include "common/resources/font/glcd.h"

class SSD1306
{
public:
    enum Command {
        SEGMENT_REMAP_RESET                 = 0xA0
        , SEGMENT_REMMAP                    = 0xA1
        , MULTIPLEX_RATIO                   = 0xA8
        , DISPLAY_OFF                       = 0xAE
        , DISPLAY_ON                        = 0xAF
        , DISPLAY_OFFSET                    = 0xD3
        , CLOCK_DIVIDE_OSCILLATOR_FREQUENCY = 0xD5
    };

    SSD1306( I2C *bus, uint8_t control );
    ~SSD1306();

    void initialize();

    int32_t writeCommand( int32_t numBytes, ... );
    int32_t writeCommand( Command command, const uint8_t *buffer, uint32_t size );

    int32_t writeText( const char *text, int8_t row, int8_t col );

    void clearBuffer();
    void clearScreen();
    bool writeScreen();

private:
    I2C *mI2CBus;

    uint8_t mControlAddress;
    uint8_t *mScreenBuffer;

    static const char *hello_world;
    static const uint8_t control_address;
    static const uint32_t height;
    static const uint32_t width;
};

#endif // SSD1306_H
