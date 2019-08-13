/** ****************************************************************************
 * @file register.h
 * @author Trevor Horst
 * @copyright None
 * @brief Register definitions
 * @note Code was inspried by this post:
 * https://stackoverflow.com/questions/11815894/how-to-read-write-arbitrary-bits-in-c-c
 * ****************************************************************************/

#ifndef REGISTER_H
#define REGISTER_H

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

// Creates a bitfield of given size and index
#define BITFIELD( INDEX, SIZE ) \
    ( ( ( 1UL << ( SIZE ) ) - 1UL) << ( INDEX ) )

// Reads a specified bitfield from a register
#define REGISTER_READ( REG, INDEX, SIZE ) \
    ( ( ( REG ) & BITFIELD( ( INDEX ), ( SIZE ) ) ) >> ( INDEX ) )

// Writes a specified value to a specified bitfield
#define REGISTER_WRITE( REG, INDEX, SIZE, VALUE ) \
    ( REG = ( ( REG ) & ( ~BITFIELD( INDEX, SIZE ) ) ) \
    | ( static_cast< decltype( REG ) >( ( VALUE ) << INDEX ) & BITFIELD( INDEX, SIZE ) ) )

// Creates getters and setters for a regiseter field
#define REGISTER_FIELD( REG, NAME, INDEX, SIZE ) \
  inline decltype( REG ) NAME() volatile { return REGISTER_READ( REG, INDEX, SIZE ); } \
  inline void set_##NAME( decltype( REG ) value ) volatile { REGISTER_WRITE( REG, INDEX, SIZE, value); }

static const unsigned int high = 1;
static const unsigned int low  = 0;

/**
 * @brief 64 bit register
 */
struct __attribute__ ((__packed__)) Register64 {
    unsigned long int mData = 0;
};

/**
 * @brief 48 bit register
 */
struct __attribute__ ((__packed__)) Register48 {
    unsigned short x0 = 0;
    unsigned short x2 = 0;
    unsigned short x4 = 0;
};

/**
 * @brief 32 bit register
 */
struct __attribute__ ((__packed__)) Register32 {
    unsigned int mData = 0;
};

/**
 * @brief 16 bit register
 */
struct __attribute__ ((__packed__)) Register16 {
    unsigned short mData = 0;
};

#define REGISTER64( NAME ) \
    struct NAME : public Register64

#define REGISTER32( NAME ) \
    struct NAME : public Register32

#define REGISTER16( NAME ) \
    struct NAME : public Register16


template< class T >
class MemoryMappedRegister
{
    static const char memory_device[];
public:

    /**
     * @brief Constructor
     * @param offset Address offset of the register in memory
     * @param simulated Is the memory simulated
     */
    MemoryMappedRegister( unsigned int offset, bool simulated = false )
        : mSimulated( simulated )
        , mOffset( offset )
        , mMap( nullptr )
    {
        mapInterface();
    }

    /**
     * @brief Destructor
     */
    ~MemoryMappedRegister()
    {
        unmapInterface();
    }

    /**
     * @brief Maps the memory interface
     * @return Error code
     */
    int mapInterface()
    {
        int err = 0;

        unmapInterface();

        if( mSimulated ) {
            // If this is a simulation, just fake the memory
            mMap = new T();
        } else {
            // If this isn't simulated, let's try and open the memory device
            int fileDescriptor = open( memory_device, O_RDWR | O_SYNC
                           , static_cast< mode_t >( 0600 ) );

            if( fileDescriptor < 0 ) {
                printf( "Cannot open %s.\n", memory_device );
                printf( "Error %d: %s\n", errno, strerror( errno ) );
                err = errno;
            } else {

                // Starting address
                void *addr = nullptr;
                // Length of mapping
                size_t length = sizeof( T );
                // Pages may be read/written
                int prot = PROT_READ | PROT_WRITE;
                // Updates to mapping are shared amongst other mapping to the
                // same region
                int flags = MAP_SHARED;

                void *mem = mmap( addr, length, prot, flags, fileDescriptor
                                  , mOffset );

                if( mem == MAP_FAILED ) {
                    // Memory map failed
                    printf( "mmap failed. addr: %p\n", addr );
                    printf( "Error %d: %s\n", errno, strerror( errno ) );
                    err = errno;
                } else {
                    // Memory map succeeded, cast the pointer
                    mMap = static_cast< T* >( mem );
                }

                close( fileDescriptor );
            }
        }

        return err;
    }

    /**
     * @brief Unmaps the memory interface
     * @return Error code
     */
    int unmapInterface()
    {
        int err = 0;
        if( mMap ) {
            if( mSimulated ) {
                // If simulated, delete the fake memory space
                delete mMap;
            } else {
                // If not simulated, unmap the memory space
                size_t length = sizeof( T );
                // Cast out the volatile, not sure if this is undefined behavior
                // or not. Need more information
                T *map = const_cast< T* >( mMap );
                err = munmap( static_cast< void* >( map ), length );
            }

            // Set the map to a null pointer
            mMap = nullptr;
        }
        return err;
    }

    volatile T *map()
    {
        return mMap;
    }

protected:
    bool mSimulated;
    unsigned int mOffset;
    volatile T *mMap;

private:
};


template< class T >
const char MemoryMappedRegister< T >::memory_device[] = "/dev/mem";

#endif // REGISTER_H
