#include "common/timer.h"

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

        // Attempt to join the thread
        if( mThread->joinable() ) {
            // Wait for the thread to finish then clean it up
            mThread->join();
            delete mThread;
            mThread = nullptr;
        } else {
            LOG_ERROR( "failed to join timer" );
        }
    }
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
    mThread = new std::thread( [ this ] {
        do {
            // Super stoked for my first legitimate use of a do while loop
            // Perform the sleep once, or forever until told to stop
            if( !mEnable ) {
                return;
            }

            std::this_thread::sleep_for( std::chrono::milliseconds( mDelay ) );

            if( !mEnable ) {
                return;
            }

            if( mCallback != nullptr ) {
                mCallback();
            }
        } while( mType == Type::INTERVAL );
    } );
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
        LOG_INFO( "timer is not active" );
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
