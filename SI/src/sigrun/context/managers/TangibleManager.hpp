

#ifndef SITEST_TANGIBLEMANAGER_HPP
#define SITEST_TANGIBLEMANAGER_HPP


#include <ostream>
#include <sigrun/tangible/SITUIOObject.hpp>

class TangibleManager: public SIObject
{ SIGRUN
public:

    TangibleManager();
    ~TangibleManager();

    void update(SITUIOObject *tobj);
    void remove(int s_id);

    const std::vector<int>& tangible_ids();

private:
    void add_tangible(SITUIOObject *tobj);
    void update_tangible(SITUIOObject *tobj);

    Region* associated_region(int s_id);

    std::vector<int> d_tangible_ids;
};


#endif //SITEST_TANGIBLEMANAGER_HPP
