#include "http/server/router.h"

namespace NewHttp {

/**
 * @brief Route constructor
 */
Route::Route()
{

}

/**
 * @brief Get HTTP verbs
 * @return Reference to to the local verbs
 */
const std::vector< NewHttp::Method > &Route::getVerbs() const
{
    return mVerbs;
}

/**
 * @brief Set the local HTTP verbs
 * @param verbs Desired verbs to set
 * @return
 */
uint32_t Route::setVerbs( const std::vector<NewHttp::Method> &verbs )
{
    mVerbs = verbs;
    return 0;
}

/**
 * @brief Set the local action
 * @param action Desired action
 * @return
 */
uint32_t Route::setAction( const std::string &action )
{
    mAction = action;
    return 0;
}

}
