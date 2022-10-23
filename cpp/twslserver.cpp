
#include <iostream>
#include <unistd.h>
#include <pthread.h>

#include "tsocketfactory.h"
#include "event_device_error.h"

pthread_mutex_t write_event_ = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) {
    std::cout << "\n\n\n\n\n" << std::endl;
    std::cout << "##############################################################" << std::endl;
    std::cout << "################### twslServer begins" << std::endl;

    twsl::TSocketFactory* tsf = twsl::TSocketFactory::instance();
    try {
        tsf->listen_for_connection(17303);
    } catch ( twsl::event_device_error r ) {
        std::cout << "twslServer,  " << r.reason() << std::endl;
    }
    std::cout << "############### twslServer exiting" << std::endl;
    return 0;
}

