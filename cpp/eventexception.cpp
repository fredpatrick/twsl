
#include "eventexception.h"

twsl::EventException::
EventException( const std::string& reason )
{
    reason_ = reason;
}

twsl::
EventException::
EventException ( const EventException& cf)
{
    reason_ = cf.reason_;
}

twsl::EventException&
twsl::EventException::
operator=( const EventException& cf)
{
    if ( &cf != this ) reason_ = cf.reason_;
    return *this;
}

twsl::EventException::
~EventException()
{
}

const char*
twsl::EventException::
reason() const
{
    return reason_.c_str();
} 
