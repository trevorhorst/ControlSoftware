#include "common/timer.h"

const int32_t Timer::delay_1000_ms = 1000;
const int32_t Timer::delay_100_ms = 100;

/**
 * @brief Constructor
 */
Timer::Timer( int32_t delayMs, Type type, std::function< void() > callback )
    : mEnable( false )
    , mDelay( delayMs )
    , mType( type )
    , mThread( nullptr )
    , mCallback( callback )
{

}

/**
 * @brief Destructor
 */
Timer::~Timer()
{
    if( mThread ) {
        // Issue a stop on the thread if it is still active
        if( mEnable ) {
            stop();
        }
    }
}

/**
 * @brief Run the timer thread
 */
void Timer::run()
{
    while( mEnable ) {

        int32_t totalTime = 0;
        while( totalTime < mDelay ) {
            if( !mEnable ) {
                return;
            }
            std::this_thread::sleep_for(
                        std::chrono::milliseconds( delay_100_ms ) );
            totalTime += 100;
        }

        if( !mEnable ) {
            return;
        }

        if( mCallback != nullptr ) {
            mCallback();
        }

        if( mType == Type::SINGLE_SHOT ) {
            return;
        }
    }

    stop();
}

/**
 * @brief Start the timer thread
 *
 * @todo May want to figure out if there is a better way to handle the sleep.
 * If we sleep for too long we will have to wait for the thread to come out
 * of sleep before we can join the thread. Review this post for an interruptible
 * sleep:
 * https://stackoverflow.com/questions/32233019/wake-up-a-stdthread-from-usleep
 */
void Timer::start()
{
    mEnable = true;
    mThread = new std::thread( &Timer::run, this );
}

/**
 * @brief Stop the timer, join the thread and delete it
 */
void Timer::stop()
{
    mEnable = false;
    if( mThread ) {
        if( mThread->joinable() ) {
            mThread->join();
            delete mThread;
            mThread = nullptr;
        } else {
            LOG_ERROR( "failed to join timer" );
        }
    } else {
        if( isVerbose() ){ LOG_INFO( "timer is not active" ); }
    }

}

/**
 * @brief Retreives the timer enable status
 * @return bool enable status
 */
bool Timer::isEnabled()
{
    return mEnable;
}

/**
 * @brief Sets the timer enable status
 * @param enable Desired enable status
 * @return uint32_t error cod
 */
uint32_t Timer::setEnable( bool enable )
{
    uint32_t err = Error::Code::NONE;

    if( enable ) {
        start();
    } else {
        stop();
    }

    return err;
}
