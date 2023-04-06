

#ifndef SITEST_TANGIBLEMANAGER_HPP
#define SITEST_TANGIBLEMANAGER_HPP


#include <ostream>
#include <sigrun/network/TangibleObjectMessage.hpp>
#include <queue>

#define MESSAGE_VALID(x) x && !x->shape().empty() && x->shape().size() > 2

class TangibleManager: public SIObject
{ SIGRUN


public:
    TangibleManager();
    ~TangibleManager();

    void* handle_uds(void* args);

private:

    int uds_fd = -1;    // path to uds for remotely controlled delay times
    std::string uds_path = "/home/juergen/Desktop/socket_test.s";
    pthread_t uds_thread;

    int server_socket, client_socket;
    static void* run_helper(void* obj)
    {
        TangibleManager* ptm = static_cast<TangibleManager*>(obj);
        return ptm->handle_uds(nullptr);
    }
};




#endif //SITEST_TANGIBLEMANAGER_HPP
