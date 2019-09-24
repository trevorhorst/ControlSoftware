/**
 * @file control_template.h
 * @author Trevor Horst
 * @brief The control template class allows us to keep track of all the available
 * control objects for a particular class automatically. Any time a new object
 * is created it will be pushed back into the list
 */

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
        // Add the control object to the list of control objects
        mControlObjects.push_back( static_cast< T* >( this ) );
    }

    ~ControlTemplate()
    {
        // for( auto it = mControlObjects.begin(); it != mControlObjects.end(); it++ ) {
        // }
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
