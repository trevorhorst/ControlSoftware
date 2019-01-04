#include "common/control/control.h"

Control::Control()
    : mVerbose( false )
{

}

Control::~Control()
{

}

uint32_t Control::setVerbose( bool verbose )
{
    uint32_t r = Error::Code::NONE;
    mVerbose = verbose;
    return r;
}

bool Control::isVerbose()
{
    return mVerbose;
}
