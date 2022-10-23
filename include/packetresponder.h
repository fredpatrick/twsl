
#ifndef TWSL_PACKETRESPONDER_HH
#define TWSL_PACKETRESPONDER_HH

namespace twsl {
    class PacketBuffer;
    class TSocket;

    class PacketResponder {

        public:
            PacketResponder(TSocket* tsocket, bool& shutdown);
            ~PacketResponder();

            virtual void    packet(int ierr) = 0;

            int             socket_fd();

        protected:
            int  socket_fd_;
            bool shutdown_;
        private:
    };
}
#endif
