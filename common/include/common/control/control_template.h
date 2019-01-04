#ifndef CONTROL_TEMPLATE_H
#define CONTROL_TEMPLATE_H

#include <vector>

#include "common/control/control.h"

template< typename T >
class ControlTemplate : public Control
{
public:

    /**
     * @brief Constructor
     */
    ControlTemplate()
        : Control()
        , mId( mCount++ )
    {
        mControlObjects.push_back( static_cast< T* >( this ) );
    }

    /**
     * @brief Retrieves a control object
     * @param id ID of the control object to retrieve
     * @return Pointer to the control object
     */
    static T* getControlObject( uint32_t id )
    {
        if( id + 1 <= mControlObjects.size() ) {
            return mControlObjects.at( id );
        }
        return nullptr;
    }

    static uint32_t getCount()
    {
        return mCount;
    }

    /**
     * @brief Retrieves the ID of the control object
     * @return Integer representation of the control object's ID
     */
    uint32_t getId()
    {
        return mId;
    }

private:
    static std::vector< T* > mControlObjects;
    static uint32_t mCount;
    uint32_t mId;
};

template< typename T >
std::vector< T* > ControlTemplate< T >::mControlObjects = {};

template< typename T >
uint32_t ControlTemplate< T >::mCount = 0;

#endif // CONTROL_TEMPLATE_H
