

#include <sigrun/context/Context.hpp>
#include "TangibleManager.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

// Code taken for testing from Andreas Schmid:
https://github.com/vigitia/IRPenTracking/blob/frontend_multipointer_support/sdl_frontend/uds.cpp

TangibleManager::TangibleManager()
{
    socklen_t addrlen;
    ssize_t size;
    struct sockaddr_un address;
    const int y = 1;
    if((server_socket=socket (AF_LOCAL, SOCK_STREAM, 0)) > 0)
        printf ("created socket\n");
    unlink(uds_path.c_str());
    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, uds_path.c_str());
    if (bind ( server_socket,
               (struct sockaddr *) &address,
               sizeof (address)) != 0) {
        printf( "port is not free!\n");
    }
    listen (server_socket, 5);
    addrlen = sizeof (struct sockaddr_in);
    while (1) {
        client_socket = accept ( server_socket,
                                 (struct sockaddr *) &address,
                                 &addrlen );
        if (client_socket > 0)
        {
            printf ("client connected\n");
            break;
        }
    }

    pthread_create(&uds_thread, nullptr, reinterpret_cast<void *(*)(void *)>(TangibleManager::run_helper), NULL);
}

TangibleManager::~TangibleManager() = default;

void *TangibleManager::handle_uds(void *args)
{
    while(1)
    {
        // receive header message containing the length of the message
        //char header[4];
        //header[0] = 0;
        //header[1] = 0;
        //header[2] = 0;
        //header[3] = 0;

        char* header = (char *) malloc(4 * sizeof(char));
        int header_size = recv(client_socket, header, 4, MSG_WAITALL);

        // create buffer with appropriate size
        int buffer_length = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | header[3];
        free(header);
        char buffer[buffer_length + 1];

        // receive actual message
        int size = recv(client_socket, buffer, buffer_length, MSG_WAITALL);

        // add terminating character to avoid junk in the end
        buffer[buffer_length] = '\0';

        // hand over to message parser

        // for debugging
        //if(result != 1)
        //{
        //    cout << buffer << endl;
        //}
    }

    return nullptr;
}
