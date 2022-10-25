
#ifndef TWSL_PACKETRESPONDER_HH
#define TWSL_PACKETRESPONDER_HH

namespace twsl {
    class PacketBuffer;
    class TSocket;

    class PacketResponder {

        public:
            PacketResponder(TSocket* tsocket, bool& shutdown);
            ~PacketResponder();

            void     packet(int ierr);

            int      socket_fd();
            TSocket* tsocket();

        protected:
            int      socket_fd_;
            bool     shutdown_;
            TSocket* tsocket_;
            int      packet_count_;
        private:
    };
}
#endif
