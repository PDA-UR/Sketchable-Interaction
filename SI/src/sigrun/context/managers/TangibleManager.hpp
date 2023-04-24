

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

    void start();
    void* handle_uds(void* args);

    bool is_started();

private:
    bool d_is_started;
    int uds_fd = -1;
    std::string uds_path = "/home/vigitia/Desktop/IRPenTracking/uds_test";
    pthread_t uds_thread;

    int server_socket, client_socket;
    static void* run_helper(void* obj)
    {
        TangibleManager* ptm = static_cast<TangibleManager*>(obj);
        return ptm->handle_uds(nullptr);
    }

    std::shared_ptr<Region> red;
    std::shared_ptr<Region> green;
    std::shared_ptr<Region> blue;
};




#endif //SITEST_TANGIBLEMANAGER_HPP