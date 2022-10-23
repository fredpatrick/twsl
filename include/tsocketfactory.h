
#ifndef TWSL_TSOCKETFACTORY_HH
#define TWSL_TSOCKETFACTORY_HH

namespace twsl
{
    class TSocketFactory;
    {
        class TSocket;

        public:
            
            static TSocketFactory* instance();

            ~TSocketFactory();

            void                       listen_for_connection(int portno);
            void                       stop_listening();
            TSocket*                   connect(int wsl_ip, int portno);

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
