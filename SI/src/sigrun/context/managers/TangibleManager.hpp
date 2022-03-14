

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

    void receive(const TangibleObjectMessage *p_message);
    void receive(const bp::dict& data);
    void remove(int id);

    const std::vector<int>& tangible_ids();
    void set_current_pen_selection(const std::string &effect_to_assign, const std::string &effect_display_name, const std::string &effect_texture_path, bp::dict &kwargs);

private:
    int d_update = 0;
    void add_tangible(const TangibleObjectMessage *p_message);
    void update_tangible(const TangibleObjectMessage *p_message);

    void correct_shape(std::vector<glm::vec3>& out, const std::vector<glm::vec3>& in, const glm::vec2& tracker_dimensions);
    void add_links_to_kwargs(bp::dict& kwargs, const std::vector<int>& links);

    std::shared_ptr<Region> associated_region(int s_id);

    std::queue<const TangibleObjectMessage*> d_msg_queue;

    std::vector<int> d_tangible_ids;
    std::clock_t  d_last_time, d_delta_time, d_overall_time = 0;
    std::string d_assigned_pen_effect;
    std::string d_assigned_pen_effect_display_name;
    std::string d_assigned_pen_effect_texture_path;
    bp::object d_assigned_pen_effect_kwargs;
};


#endif //SITEST_TANGIBLEMANAGER_HPP
