

#ifndef SI_SIGRUN_SITUIOObject
#define SI_SIGRUN_SITUIOObject

#include <sigrun/SIObject.hpp>
#include <ostream>
#include "../lib/tuio_headers/oscpack/osc/OscPacketListener.h"

/*
 * OCG only contains a session id (s_id).
 * Therefore, it is required to combine an ocg message with another message, which contains the same s_id and also type id t_id, user_id u_id, and component id c_id.
 * s_id allows for distinguishing multiple tracked objects to correctly assign and update their data based on user interaction.
 * In the particular case of the ocg message, SIGRun requires a sym message of the same s_id to correctly assign and update an interactive region which is assigned to a tracked contour.
 */
class SITUIOToken: public SIObject
{ SIGRUN
public:
    SITUIOToken(const osc::ReceivedMessage& m);
    ~SITUIOToken();

    int s_id();
    int t_id();
    int u_id();
    int c_id();
    float x_pos();
    float y_pos();
    float angle() const;
    float x_vel();
    float y_vel();
    float a_vel();
    float m_acc();
    float r_acc();

private:
    enum VALUES
    {
        S_ID = 0,
        TU_ID = 1,
        C_ID = 2,
        X_POS = 3,
        Y_POS = 4,
        ANGLE = 5,
        X_VEL = 6,
        Y_VEL = 7,
        A_VEL = 8,
        M_ACC = 9,
        R_ACC = 10
    };

    int d_s_id;
    int d_t_id;
    int d_u_id;
    int d_c_id;
    float d_x_pos;
    float d_y_pos;
    float d_angle;
    float d_x_vel;
    float d_y_vel;
    float d_a_vel;
    float d_m_acc;
    float d_r_acc;
};

class SITUIOPointer: public SIObject
{ SIGRUN

public:
    SITUIOPointer(const osc::ReceivedMessage& m);
    ~SITUIOPointer();

    int s_id();
    int t_id();
    int u_id();
    int c_id();
    float x_pos();
    float y_pos();
    float angle();
    float shear();
    float radius();
    float press();
    float x_vel();
    float y_vel();
    float p_vel();
    float m_acc();
    float r_acc();

private:
    enum VALUES
    {
        S_ID = 0,
        TU_ID = 1,
        C_ID = 2,
        X_POS = 3,
        Y_POS = 4,
        ANGLE = 5,
        SHEAR = 6,
        RADIUS = 7,
        PRESS = 8,
        X_VEL = 9,
        Y_VEL = 10,
        P_VEL = 11,
        M_ACC = 12,
        R_ACC = 13
    };

    int d_s_id;
    int d_t_id;
    int d_u_id;
    int d_c_id;
    float d_x_pos;
    float d_y_pos;
    float d_angle;
    float d_shear;
    float d_radius;
    float d_press;
    float d_x_vel;
    float d_y_vel;
    float d_p_vel;
    float d_m_acc;
    float d_r_acc;
};

class SITUIOBounds: public SIObject
{ SIGRUN
public:
    SITUIOBounds(const osc::ReceivedMessage& m);
    ~SITUIOBounds();

    int s_id();
    float x_pos() const;
    float y_pos();
    float angle();
    float width();
    float height();
    float area();
    float x_vel();
    float y_vel();
    float a_vel();
    float m_acc();
    float r_acc();

private:
    enum VALUES
    {
        S_ID = 0,
        X_POS = 1,
        Y_POS = 2,
        ANGLE = 3,
        WIDTH = 4,
        HEIGHT = 5,
        AREA = 6,
        X_VEL = 7,
        Y_VEL = 8,
        A_VEL = 9,
        M_ACC = 10,
        R_ACC = 11
    };

    int d_s_id;
    float d_x_pos;
    float d_y_pos;
    float d_angle;
    float d_width;
    float d_height;
    float d_area;
    float d_x_vel;
    float d_y_vel;
    float d_a_vel;
    float d_m_acc;
    float d_r_acc;
};

class SITUIOSymbol: public SIObject
{ SIGRUN

public:
    SITUIOSymbol(const osc::ReceivedMessage& m);
    ~SITUIOSymbol();

    int s_id();
    int t_id();
    int u_id();
    int c_id();

    const std::string& group() const;
    const std::string& data() const;

private:
    int d_s_id;
    int d_t_id;
    int d_u_id;
    int d_c_id;
    std::string d_group;
    std::string d_data;
};

class SITUIOOuterContourGeometry: public SIObject
{ SIGRUN
public:
    SITUIOOuterContourGeometry(const osc::ReceivedMessage& m);
    ~SITUIOOuterContourGeometry();

    int s_id();
    const std::vector<glm::vec3>& contour() const;

private:
    int d_s_id;
    std::vector<glm::vec3> d_contour;
};

class SITUIOObject : public SIObject
{ SIGRUN


public:
    SITUIOObject(int s_id, int f_id);
    ~SITUIOObject();

    void add_token_data(const osc::ReceivedMessage& m);
    void add_pointer_data(const osc::ReceivedMessage& m);
    void add_bounds_data(const osc::ReceivedMessage& m);
    void add_symbol_data(const osc::ReceivedMessage& m);
    void add_outer_contour_geometry_data(const osc::ReceivedMessage& m);

    SITUIOToken *const token_component() const;
    const SITUIOPointer* pointer_component() const;
    SITUIOBounds *const bounds_component() const;
    const SITUIOSymbol* symbol_component() const;
    const SITUIOOuterContourGeometry* outer_contour_geometry_component() const;

    bool has_token_component();
    bool has_pointer_component();
    bool has_bounds_component();
    bool has_symbol_component();
    bool has_outer_counter_geometry_component();
    bool has_any_component();

    int s_id();

private:
    int d_s_id;
    int d_f_id;

    SITUIOToken* p_token;
    SITUIOPointer* p_pointer;
    SITUIOBounds* p_bounds;
    SITUIOSymbol* p_symbol;
    SITUIOOuterContourGeometry* p_ocg;
};

#endif // SI_SIGRUN_SITUIOObject