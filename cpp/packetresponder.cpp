
#include "eventexception.h"
#include "packetresponder.h"
#include "packetbuffer.h"
#include "tsocket.h"
#include "tsocketfactory.h"
#include <pthread.h>
#include <iostream>
#include <string>

twsl::PacketResponder::
PacketResponder(TSocket* tsocket, 
                         bool& shutdown) : shutdown_(shutdown)
{
    tsocket_      = tsocket;
    socket_fd_    = tsocket_->socket_fd();
    packet_count_ = 0;
    tsocket_->wait_for_packet(this);        //  When tsocket in server mode receives a packet
                                            // then packet will be called to respond
    std::cout << "PacketResponder.ctor, tsocket thread created" << std::endl;
    return;
}

twsl::PacketResponder::
~PacketResponder()
{
}

twsl::TSocket*
twsl::PacketResponder::
tsocket()
{
    return tsocket_;
}

int
twsl::PacketResponder::
socket_fd()
{
    return socket_fd_;
} 

void
twsl::PacketResponder::
packet(int ierr)
{
    PacketBuffer* pbfr;
    try {
        pbfr = tsocket_->read();
    } catch ( EventException r ) {
        std::cout << "PacketResponder.packet: " << r.reason() << std::endl;
        std::cout << "PacketResponder.packet, deleting drivers" << std::endl;
        ::pthread_exit(0);
    }

    std::string tag = pbfr->tag();
    std::cout << "PacketResponder.packet, received packet, tag = " << tag << std::endl;
    if ( tag == "C" ) {
        std::string clnt_msg = pbfr->strdat();
        std::cout << "PacketResponder.packet, client message is: " << clnt_msg << std::endl;
        delete pbfr;
        packet_count_++;
        pbfr = new PacketBuffer("S");
        pbfr->intdat(packet_count_);
        pbfr->strdat(clnt_msg);
        tsocket_->write(pbfr);

    } else if ( tag == "Q" ) {
        TSocketFactory* tf = TSocketFactory::instance();
        tf->stop_listening();
        shutdown_ = true;
        ::pthread_exit(0);
    }
}

