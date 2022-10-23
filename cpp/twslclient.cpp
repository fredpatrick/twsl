#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <bitset>
#include <iostream>

#include "tsocketfactory.h"
#include "tsocket.h"
#include "packetbuffer.h"

#include <cmath>

int main ( int argc, char* argv[])
{
    std::cout << "#################################### twslClient " << std::endl;
    std::cout << "Enter wsl_ip from server:";
    std::string wsl_ip;
    std::cin >> wsl_ip;
    twsl::TSocketFactory* tsf     = twsl::TSocketFactory.instance();
    twsl::TSocket*        tsocket = tsf->connect(wsl_ip,17303);
    std::cout << "twslClient, connection made to " << wsl_ip << std::endl;

    bool finished = false;
    while ( !finished ) {
        std::string cmd;
        std::cout << "Enter continue(c) or quit (q): ";
        std::cin  >> cmd;
        if (cmd == "c") {
            twsl::PacketBuffer* pbfr =  twsl::PacketBuffer("C");
            stdf::cout << "twslClient: Enter mesaage: ";
            std::string message;
            std::cin >> message;
            pbfr->strdat(message);
            tsocket->write(pbfr);
            delete pbfr;

            pbfr = tsocket->read();

            std::cout << "twslClient, return message received" << std::endl;
            int packet_count           = pbfr->intdat();
            std::string return_message = pbfr->strdat();
            std::cout << "twslClient: packet_count = "   << packet_count << std::endl;
            std::cout << "twslClient: return_message = " << return_message << std::endl;
            tsocket->write(pbfr);
            delete pbfr;
        } else if ( cmd == "q" ) {
            twsl::PacketBuffer* pbfr = new twsl::PacketBuffer("Q");
            tsocket->write(pbfr);
            std::cout << "twslClient, shutdown command sent to twslServer" << std::endl;
            delete pbfr;
            finished = true;
        }
    }
    return 0;
}


