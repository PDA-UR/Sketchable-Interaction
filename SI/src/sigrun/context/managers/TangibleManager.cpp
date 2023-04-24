

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
//https://github.com/vigitia/IRPenTracking/blob/frontend_multipointer_support/sdl_frontend/uds.cpp

TangibleManager::TangibleManager():
        d_is_started(false)
{

}

void TangibleManager::start()
{
    for (int i = 0; i < 3; i++)
    {
        bp::dict kwargs;
        kwargs["id"] = i;

        float x = i * 18;
        float y = 0;
        float w = 18 / 8;
        float h = 24 / 8;

        std::vector<glm::vec3> contour = {glm::vec3(x, y, 1), glm::vec3(x, y + h, 1), glm::vec3(x + w, y + h, 1), glm::vec3(x + w, y, 1)};

        Context::SIContext()->register_new_region_via_name(contour, "__ Tip __", false, kwargs);
    }

    d_is_started = true;

    socklen_t addrlen;

    ssize_t size;

    struct sockaddr_un address;

    const int y = 1;

    if((server_socket = socket (AF_LOCAL, SOCK_STREAM, 0)) > 0)
        Print::print("Socket created.");

    unlink(uds_path.c_str());
    address.sun_family = AF_LOCAL;

    strcpy(address.sun_path, uds_path.c_str());

    if (bind(server_socket, (struct sockaddr *) &address, sizeof (address)) != 0)
        Print::print("Port is not free!");

    listen(server_socket, 5);

    addrlen = sizeof (struct sockaddr_in);

    while(true) {
        client_socket = accept (server_socket, (struct sockaddr *) &address, &addrlen);

        if (client_socket > 0)
        {
            Print::print("Client connected!");
            break;
        }
    }

    pthread_create(&uds_thread, nullptr, reinterpret_cast<void *(*)(void *)>(TangibleManager::run_helper), this);
}

TangibleManager::~TangibleManager() = default;

void *TangibleManager::handle_uds(void *args)
{
    while(true)
    {
        if (!red && !green && !blue)
        {
            PythonGlobalInterpreterLockGuard g;

            for(auto& r: Context::SIContext()->region_manager()->regions())
            {
                if (r->name() == "__ Tip __")
                {
                    HANDLE_PYTHON_CALL(PY_ERROR, "STH IS MAJORLY BROKEN!",
                        PythonGlobalInterpreterLockGuard g;
                        uint8_t id = bp::extract<int>(r->raw_effect().attr("id"));

                        switch(id)
                        {
                            case 0: red = r; break;
                            case 1: green = r; break;
                            case 2: blue = r; break;
                        }
                    )
                }
            }

            continue;
        }


        char* header = (char *) malloc(4 * sizeof(char));
        int header_size = recv(client_socket, header, 4, MSG_WAITALL);

        // create buffer with appropriate size
        int buffer_length = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | header[3];
        free(header);
        char buffer[buffer_length + 1];

        int size = recv(client_socket, buffer, buffer_length, MSG_WAITALL);

        buffer[buffer_length] = '\0';

        int oid, x, y, state;
        uint32_t r;
        uint32_t g;
        uint32_t b;

        if(sscanf(buffer, "l %d %u %u %u %d %d %d ", &oid, &r, &g, &b, &x, &y, &state) == 7)
        {
            int id = r > 0 ? 0 : g > 0 ? 1 : b > 0 ? 2: -1;

            if (id == -1)
                continue;

            if(id == 0)
            {
                PythonGlobalInterpreterLockGuard g;
                red->raw_effect().attr("__update__")(oid, x, y, state);
            }
        }
    }

    return nullptr;
}

bool TangibleManager::is_started()
{
    return d_is_started;
}