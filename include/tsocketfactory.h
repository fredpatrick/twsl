
#ifndef TWSL_TSOCKETFACTORY_HH
#define TWSL_TSOCKETFACTORY_HH

#include <string>

namespace twsl
{
    class TSocket;

    class TSocketFactory
    {

        public:
            static TSocketFactory* instance();
            ~TSocketFactory();

            void                       listen_for_connection(int portno);
            void                       stop_listening();
            TSocket*                   connect(const std::string& wsl_ip, int portno);

        protected:
            TSocketFactory();
        private:

            int                         portno_;
            bool                        shutdown_;
            int                         listen_fd_;

            static TSocketFactory* instance_;
    };
}
#endif
