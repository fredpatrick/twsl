
#include "tsocket.h"
#include "packetresponder.h"
#include "packetbuffer.h"
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

twsl::TSocket::
TSocket(int socket_fd) : socket_fd_(socket_fd)
{
    socket_type_ = "server";
}

twsl::TSocket::
TSocket(const std::string& host_ip_addr, int portno)
{
    socket_type_ = "client";
    socket_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "TSocket.ctor, socket_fd = " << socket_fd_ << 
                              " socket_type = " << socket_type_ << std::endl;
    struct sockaddr_in     serv_addr;
    ::memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host_ip_addr.c_str());
    serv_addr.sin_port        = htons(portno);

    int ie = ::connect(socket_fd_, (struct sockaddr*)&serv_addr, sizeof(serv_addr) );
    std::cout << "TSocket.ctor-connected, ie = " << ie << std::endl;
}

twsl::TSocket::
~TSocket()
{
    std::cout << "TSocket.dtor" << std::endl;
    ::shutdown ( socket_fd_, SHUT_RDWR);
}

int
twsl::TSocket::
socket_fd()
{
    return socket_fd_;
}

int
twsl::TSocket::
write(PacketBuffer* ebfr)
{
    int bfrlen = ebfr->bfrlen();
    char* bfr  = ebfr->bfr();
                                          // This solely for the purpose of recording
    std::string tag = bfr;                //  kind of packet is still being written
    std::cout << "TSocket.write, tag = " << tag << std::endl;

    int nl = ::write( socket_fd_, &bfrlen, sizeof(int) );
    if ( nl != sizeof(int) ) {
        std::ostringstream ost;
        ost << "TSocket:write, error writing bfrlen, ns = " << nl;
        throw EventException(ost.str() );
    }
    int ns = ::write( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen) {
        std::ostringstream ost;
        ost << "TSocket:write, error writing bfr, ns = " << ns;
        throw EventException(ost.str() );
    }
    return ns;
}

twsl::PacketBuffer* 
twsl::TSocket::
read()
{
    int bfrlen;
    int nl;
    nl = ::read(socket_fd_, &bfrlen, sizeof(int));
    if ( nl == 0 )  {
        std::ostringstream ost;
        ost << "TSocket:read, socket_fd = " << socket_fd_ << 
                                     ", error reading bfrlen, end of file";
        throw EventException(ost.str() );
    }
    char* bfr = new char[bfrlen];
    int ns = ::read( socket_fd_, bfr, bfrlen);
    if ( ns != bfrlen ) {
        std::ostringstream ost;
        ost << "TSocket:read, error reading bfr, bfrlen = " << bfrlen << ", ns = " << ns;
        throw EventException(ost.str() );
    }

    std::string tag = bfr;
    PacketBuffer* ebfr = new PacketBuffer(bfrlen, bfr); 
    delete[] bfr;
    return ebfr;
}

int
twsl::TSocket::
wait_for_packet(PacketResponder* packet_responder)
{
    std::cout << "TSocket:wait_for_packed called, socket_fd = " << socket_fd_ << std::endl;
    thread_running_ = true;
    if( ::pthread_create(&packet_thread_, NULL,
                         &threaded_poll,
                         static_cast<void*>(packet_responder)) ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "TSocket::wait_for_packet, unable to create pthread, \"" << errmsg << "\"";
        throw EventException(ost.str() );
    }
    std::cout << "TSocket.wait_for_packet returning" << std::endl;
    return 0;
}

void*
twsl::TSocket::
threaded_poll(void* attr)
{
    std::cout << "TSocket.threaded_poll-begin" << std::endl;
    PacketResponder* pr = static_cast<PacketResponder*>(attr);
    TSocket* tsocket = pr->tsocket();
    while ( tsocket->thread_running_ ) {
        pr->packet(tsocket->wait_for_packet() );
        std::cout << "TSocket.threaded_poll processed packet" << std::endl;
    }
    return 0;
}

int
twsl::TSocket::
wait_for_packet()
{
    int epollfd = epoll_create(1);
    if ( epollfd == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "TSocket::wait_for_packet, unable to create epoll, " <<
                                         "\"" << errmsg << "\"";
        throw EventException(ost.str() );
    }
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLPRI;
    ev.data.fd= socket_fd_;

    if ( epoll_ctl( epollfd, EPOLL_CTL_ADD, socket_fd_, &ev) == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "TSocket::wait_for_packet, unable to add epoll ctl interface, " <<
                                         "\"" << errmsg << "\"";
        throw EventException(ost.str() );
    }

    int ie = epoll_wait(epollfd, &ev, 1, -1);
    if ( ie == -1 ) {
        char* errmsg = strerror(errno);
        std::ostringstream ost;
        ost << "TSocket::wait_for_packet, epoll wait failed, " <<
                                         "\"" << errmsg << "\"";
        throw EventException(ost.str() );
    }
    std::cout << "TSocket::wait_for_packet(), received event" << std::endl;
    return 0;
}

int
twsl::TSocket::
wait_for_exit()
{
    void*   res;
    thread_running_ = false;
    int ie = ::pthread_join(packet_thread_, &res);
    if ( ie == -1 ) {
        char* errmsg = ::strerror(errno);
        std::ostringstream ost;
        ost << "TSocket::wait_for_exit, pthread join failed, " <<
                                         "\"" << errmsg << "\"";
        throw EventException(ost.str() );
    }
    std::cout << "TSocket.wait_for_exit, after ::pthread_join" << std::endl;
    return 0;
}
