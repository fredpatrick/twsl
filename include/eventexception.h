
#ifndef TWSL_EVENTEXCEPTION_HH
#define TWSL_EVENTEXCEPTION_HH

#include <string>

namespace twsl
{

    class EventException
    {
        public:
            EventException(const std::string& reason);
            ~EventException();
     
            const char* reason() const;
            EventException ( const EventException&);
            EventException& operator=( const EventException&);
        private:
            std::string reason_;
    };

}

#endif
