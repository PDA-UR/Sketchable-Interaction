

#ifndef SI_SIGRUN_GenericMessage
#define SI_SIGRUN_GenericMessage

#include <sigrun/SIObject.hpp>

class TangibleObjectMessage: public SIObject
{ SIGRUN
public:
    TangibleObjectMessage(int32_t id, const std::vector<glm::vec3> &shape, const std::string &plugin_identifier,
                          float x, float y, const glm::vec4& color, bool is_click, bool is_drag, bool is_dbl_click, bool is_touch, bool is_alive,
                          const std::vector<int>& links, int tracker_dimension_x, int tracker_dimension_y);
    ~TangibleObjectMessage();

    void send();

    const int id() const;
    const std::vector<glm::vec3>& shape() const;
    const std::string& plugin_identifier() const;
    const float x() const;
    const float y() const;
    const glm::vec4& color() const;
    const bool is_click() const;
    const bool is_drag() const;
    const bool is_dbl_click() const;
    const bool is_touch() const;
    const bool is_alive() const;
    const std::vector<int>& links() const;
    const glm::vec2& tracker_dimensions() const;
    const int tracker_dimension_x() const;
    const int tracker_dimension_y() const;
    const bool has_links() const;

private:
    glm::vec4 d_color;
    int32_t d_id;
    std::vector<glm::vec3> d_shape;
    std::string d_plugin_identifier;
    float d_x;
    float d_y;
    bool d_is_click;
    bool d_is_drag;
    bool d_is_dbl_click;
    bool d_is_touch;
    bool d_is_alive;
    std::vector<int> d_links;
    glm::vec2 d_tracker_dimensions;

};

#endif // SI_SIGRUN_GenericMessage