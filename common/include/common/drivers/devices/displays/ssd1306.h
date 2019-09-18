#ifndef SSD1306_H
#define SSD1306_H

#include "common/drivers/i2c.h"

class SSD1306
{
public:
    SSD1306( I2C *bus, uint8_t control );
    ~SSD1306();

    void initialize();

    int32_t writeCommand( int32_t numBytes, ... );

    void clearBuffer();
    void clearScreen();
    bool writeScreen();

private:
    I2C *mI2CBus;

    uint8_t mControlAddress;
    uint8_t *mScreenBuffer;

    static const uint8_t control_address;
    static const uint32_t height;
    static const uint32_t width;
};

#endif // SSD1306_H
