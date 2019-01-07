#include "common/system/datetime.h"

/**
 * @brief Constructor
 */
DateTime::DateTime()
{

}

/**
 * @brief Retrieves the date and time
 * @return String representation of the date and time
 */
const char *DateTime::getDateTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t( now );
    return std::ctime( &time );
}
