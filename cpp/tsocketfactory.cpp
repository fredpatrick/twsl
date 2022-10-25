#include "tsocketfactory.h"
#include "tsocket.h"
#include "packetbuffer.h"
#include "packetresponder.h"
#include "eventexception.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <sstream>

twsl::TSocketFactory* twsl::TSocketFactory::instance_ = 0;

twsl::TSocketFactory::
TSocketFactory()
{
    listen_fd_           = -1;
}

twsl::TSocketFactory*
twsl::TSocketFactory::
instance()
{
    if ( instance_ == 0 ) {
        instance_ = new TSocketFactory();
    }
    return instance_;
}

twsl::TSocketFactory::
~TSocketFactory()
{
    
}

void
twsl::TSocketFactory::
listen_for_connection(int portno)
{
    if ( listen_fd_ != -1 ) return;
    shutdown_            = false;

    portno_ = portno;
    std::cout << "TSocketFactory.listen_for_connection, portno = " << portno_ << std::endl;
    listen_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if ( listen_fd_ == -1) {
        ::perror("socket");
        // need to throw exception here
    }
    std::cout << "TSocketFactory.listen_for_connection, listen_fd = " << 
                                                           listen_fd_ << std::endl;
    struct sockaddr_in      serv_addr;
    ::memset(&serv_addr, 0, sizeof(serv_addr) );
    serv_addr.sin_family        = AF_INET;
    serv_addr.sin_addr.s_addr   = htonl(INADDR_ANY);
    serv_addr.sin_port          = htons(portno_);
    int ierr = ::bind(listen_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    if ( ierr == -1 ) {
        ::perror( "bind");
        throw EventException("TSocketFactory socket");
    }

    std::cout << "TSocketFactory.listen_for_connection, listening on portno = " << 
                                            portno_ << std::endl;
    ::listen(listen_fd_, 10);
    std::cout << "TSocketFactory.listen_for_connection, listen_fd = " << 
                                                           listen_fd_ << std::endl;

    while (!shutdown_) {
        std::cout << "TSocketFactory.listen_for_connection, listen_fd = " << 
                                                               listen_fd_ << std::endl;
        int socket_fd = accept(listen_fd_, (struct sockaddr*)NULL, NULL);
        if ( socket_fd == -1 ) {
            std::cout << "TSocketFactory.listen_for_connection, accept error: " << 
                                                  ::strerror(errno) << std::endl;
            throw EventException("TSocketFactory.listen_for_connection accept error");
        }
        std::cout << "TSocketFactory.listen_for_connection, connected on portno = " << 
                                   portno_ << ", socket_fd = " << socket_fd << std::endl;

        TSocket* sckt = new TSocket(socket_fd);
        PacketResponder* pr = new PacketResponder(sckt, shutdown_);
        sckt->wait_for_packet(pr);
    }
    std::cout << "TSocketFactory::listening, loop exitted" << std::endl;
    return;
}

void 
twsl::TSocketFactory::
stop_listening()
{
    std::cout << "TSocketFactory::stop_listening, listen_fd = " << listen_fd_ << std::endl;
    ::shutdown(listen_fd_, SHUT_RDWR);
}

twsl::TSocket*
twsl::TSocketFactory::
connect(const std::string& wsl_ip, int portno)
{
    TSocket*tsckt = new TSocket(wsl_ip, portno);
    return tsckt;
}

