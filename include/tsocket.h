
#ifndef TWSL_SOCKET_HH
#define TWSL_SOCKET_HH

#include <string>
#include <pthread.h>

namespace twsl
{
    class PacketResponder;
    class PacketBuffer;

    class TSocket
    {
        public:
            TSocket(int   socket_fd);
            TSocket(const std::string& host_ip_addr,
                          int          portno);
            ~TSocket();

            int              socket_fd();
            int              write(PacketBuffer* ebfr);
            PacketBuffer*    read();
            int              wait_for_packet(PacketResponder* packet_responder);
            int              wait_for_exit();
            int              wait_for_packet();
        private:
            int              socket_fd_;
            std::string      socket_type_;

            static void*     threaded_poll(void* attr);
            pthread_t        packet_thread_;
            bool             thread_running_;
            PacketResponder* packet_responder_;
    };
}
#endif
