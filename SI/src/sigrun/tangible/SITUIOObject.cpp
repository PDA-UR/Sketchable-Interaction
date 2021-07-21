

#include "SITUIOObject.hpp"

SITUIOToken::SITUIOToken(const osc::ReceivedMessage &m)
{
    int i = 0;
    for(auto it = m.ArgumentsBegin(); it != m.ArgumentsEnd(); ++it, ++i)
    {
        switch (i)
        {
            case SITUIOToken::VALUES::S_ID: d_s_id = it->AsInt32(); break;
            case SITUIOToken::VALUES::TU_ID:
            {
                int tu_id = it->AsInt32();
                d_u_id = tu_id >> 16;
                d_t_id = tu_id & 0xFFFF;
            }
                break;
            case SITUIOToken::VALUES::C_ID: d_c_id = it->AsInt32(); break;
            case SITUIOToken::VALUES::X_POS: d_x_pos = it->AsFloat(); break;
            case SITUIOToken::VALUES::Y_POS: d_y_pos = it->AsFloat(); break;
            case SITUIOToken::VALUES::ANGLE: d_angle = it->AsFloat(); break;
            case SITUIOToken::VALUES::X_VEL: d_x_vel = it->AsFloat(); break;
            case SITUIOToken::VALUES::Y_VEL: d_y_vel = it->AsFloat(); break;
            case SITUIOToken::VALUES::A_VEL: d_a_vel = it->AsFloat(); break;
            case SITUIOToken::VALUES::M_ACC: d_m_acc = it->AsFloat(); break;
            case SITUIOToken::VALUES::R_ACC: d_r_acc = it->AsFloat(); break;
        }
    }
}

SITUIOToken::~SITUIOToken() = default;

int SITUIOToken::s_id()
{
    return d_s_id;
}

int SITUIOToken::t_id()
{
    return d_t_id;
}

int SITUIOToken::u_id()
{
    return d_u_id;
}

int SITUIOToken::c_id()
{
    return d_c_id;
}

float SITUIOToken::x_pos()
{
    return d_x_pos;
}

float SITUIOToken::y_pos()
{
    return d_y_pos;
}

float SITUIOToken::angle() const
{
    return d_angle;
}

float SITUIOToken::x_vel()
{
    return d_x_vel;
}

float SITUIOToken::y_vel()
{
    return d_y_vel;
}

float SITUIOToken::a_vel()
{
    return d_a_vel;
}

float SITUIOToken::m_acc()
{
    return d_m_acc;
}

float SITUIOToken::r_acc()
{
    return d_r_acc;
}

SITUIOPointer::SITUIOPointer(const osc::ReceivedMessage& m)
{
    int i = 0;
    for(auto it = m.ArgumentsBegin(); it != m.ArgumentsEnd(); ++it, ++i)
    {
        switch (i)
        {
            case SITUIOPointer::VALUES::S_ID:
                d_s_id = it->AsInt32();
                break;
            case SITUIOPointer::VALUES::TU_ID:
            {
                int tu_id = it->AsInt32();

                d_u_id = tu_id >> 16;
                d_t_id = tu_id & 0xFFFF;
            }
            break;
            case SITUIOPointer::VALUES::C_ID: d_c_id = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::X_POS: d_x_pos = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::Y_POS: d_y_pos = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::ANGLE: d_angle = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::SHEAR: d_shear = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::RADIUS: d_radius = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::PRESS: d_press = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::X_VEL: d_x_vel = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::Y_VEL: d_y_vel = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::P_VEL: d_p_vel = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::M_ACC: d_m_acc = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
            case SITUIOPointer::VALUES::R_ACC: d_r_acc = it->IsInt32() ? it->AsInt32() : it->AsFloat(); break;
        }
    }
}

SITUIOPointer::~SITUIOPointer() = default;

int SITUIOPointer::s_id()
{
    return d_s_id;
}

int SITUIOPointer::t_id()
{
    return d_t_id;
}

int SITUIOPointer::u_id()
{
    return d_u_id;
}

int SITUIOPointer::c_id()
{
    return d_c_id;
}

float SITUIOPointer::x_pos()
{
    return d_x_pos;
}

float SITUIOPointer::y_pos()
{
    return d_y_pos;
}

float SITUIOPointer::angle()
{
    return d_angle;
}

float SITUIOPointer::shear()
{
    return d_shear;
}

float SITUIOPointer::radius()
{
    return d_radius;
}

float SITUIOPointer::press()
{
    return d_press;
}

float SITUIOPointer::x_vel()
{
    return d_x_vel;
}

float SITUIOPointer::y_vel()
{
    return d_y_vel;
}

float SITUIOPointer::p_vel()
{
    return d_p_vel;
}

float SITUIOPointer::m_acc()
{
    return d_m_acc;
}

float SITUIOPointer::r_acc()
{
    return d_r_acc;
}

SITUIOBounds::SITUIOBounds(const osc::ReceivedMessage &m)
{
    int i = 0;
    for(auto it = m.ArgumentsBegin(); it != m.ArgumentsEnd(); ++it, ++i)
    {
        switch (i)
        {
            case SITUIOBounds::VALUES::S_ID: d_s_id = it->AsInt32(); break;
            case SITUIOBounds::VALUES::X_POS: d_x_pos = it->AsFloat(); break;
            case SITUIOBounds::VALUES::Y_POS: d_y_pos = it->AsFloat(); break;
            case SITUIOBounds::VALUES::ANGLE: d_angle = it->AsFloat(); break;
            case SITUIOBounds::VALUES::WIDTH: d_width = it->AsFloat(); break;
            case SITUIOBounds::VALUES::HEIGHT: d_height = it->AsFloat(); break;
            case SITUIOBounds::VALUES::AREA: d_area = it->AsFloat(); break;
            case SITUIOBounds::VALUES::X_VEL: d_x_vel = it->AsFloat(); break;
            case SITUIOBounds::VALUES::Y_VEL: d_y_vel = it->AsFloat(); break;
            case SITUIOBounds::VALUES::A_VEL: d_a_vel = it->AsFloat(); break;
            case SITUIOBounds::VALUES::M_ACC: d_m_acc = it->AsFloat(); break;
            case SITUIOBounds::VALUES::R_ACC: d_r_acc = it->AsFloat(); break;
        }
    }
}

SITUIOBounds::~SITUIOBounds()
{

}

int SITUIOBounds::s_id()
{
    return d_s_id;
}

float SITUIOBounds::x_pos() const
{
    return d_x_pos;
}

float SITUIOBounds::y_pos()
{
    return d_y_pos;
}

float SITUIOBounds::angle()
{
    return d_angle;
}

float SITUIOBounds::width()
{
    return d_width;
}

float SITUIOBounds::height()
{
    return d_height;
}

float SITUIOBounds::area()
{
    return d_area;
}

float SITUIOBounds::x_vel()
{
    return d_x_vel;
}

float SITUIOBounds::y_vel()
{
    return d_y_vel;
}

float SITUIOBounds::a_vel()
{
    return d_a_vel;
}

float SITUIOBounds::m_acc()
{
    return d_m_acc;
}

float SITUIOBounds::r_acc()
{
    return d_r_acc;
}

SITUIOSymbol::SITUIOSymbol(const osc::ReceivedMessage &m)
{
    int s_id, tu_id, c_id;
    const char* group, * data;

    m.ArgumentStream() >> d_s_id >> tu_id >> d_c_id >> group >> data >> osc::EndMessage;

    d_u_id = tu_id >> 16;
    d_t_id = tu_id & 0xFFFF;

    d_group = std::string(group);
    d_data = std::string(data);
}

SITUIOSymbol::~SITUIOSymbol() = default;

int SITUIOSymbol::s_id()
{
    return d_s_id;
}

int SITUIOSymbol::t_id()
{
    return d_t_id;
}

int SITUIOSymbol::u_id()
{
    return d_u_id;
}

int SITUIOSymbol::c_id()
{
    return d_c_id;
}

const std::string &SITUIOSymbol::group() const
{
    return d_group;
}

const std::string &SITUIOSymbol::data() const
{
    return d_data;
}

SITUIOOuterContourGeometry::SITUIOOuterContourGeometry(const osc::ReceivedMessage &m)
{
    if(m.ArgumentCount() & 1)
    {
        d_s_id = m.ArgumentsBegin()->AsInt32();

        int i = 0, x;

        for(auto it = m.ArgumentsBegin(); it != m.ArgumentsEnd(); ++it, ++i)
        {
            if(i)
            {
                if(i & 1)
                    x = it->AsFloat();
                else
                    d_contour.emplace_back(x, it->AsFloat(), 1);
            }
        }
    }
    else
        throw;
}

SITUIOOuterContourGeometry::~SITUIOOuterContourGeometry() = default;

int SITUIOOuterContourGeometry::s_id()
{
    return d_s_id;
}

const std::vector<glm::vec3> &SITUIOOuterContourGeometry::contour() const
{
    return d_contour;
}

SITUIOLinkAssociation::SITUIOLinkAssociation(const std::vector<int>& link_associations):
    d_link_associations(link_associations)
{}

SITUIOLinkAssociation::~SITUIOLinkAssociation()
{}

const std::vector<int> &SITUIOLinkAssociation::link_associations()
{
    return d_link_associations;
}

SITUIOObject::SITUIOObject(int s_id, int f_id):
    d_s_id(s_id),
    d_f_id(f_id)
{}

SITUIOObject::~SITUIOObject()
{
    if(p_token)
    {
        delete p_token;
        p_token = nullptr;
    }
}

void SITUIOObject::add_token_data(const osc::ReceivedMessage& m)
{
    if(p_token)
    {
        delete p_token;
        p_token = nullptr;
    }

    p_token = new SITUIOToken(m);
}

void SITUIOObject::add_pointer_data(const osc::ReceivedMessage& m)
{
    if(p_pointer)
    {
        delete p_pointer;
        p_pointer = nullptr;
    }

    p_pointer = new SITUIOPointer(m);
}

void SITUIOObject::add_bounds_data(const osc::ReceivedMessage &m)
{
    if(p_bounds)
    {
        delete p_bounds;
        p_bounds = nullptr;
    }

    p_bounds = new SITUIOBounds(m);
}

void SITUIOObject::add_symbol_data(const osc::ReceivedMessage &m)
{
    if(p_symbol)
    {
        delete p_symbol;
        p_symbol = nullptr;
    }

    p_symbol = new SITUIOSymbol(m);
}

void SITUIOObject::add_outer_contour_geometry_data(const osc::ReceivedMessage &m)
{
    if(p_ocg)
    {
        delete p_ocg;
        p_ocg = nullptr;
    }

    p_ocg = new SITUIOOuterContourGeometry(m);
}

void SITUIOObject::add_link_association_data(const std::vector<int>& link_associations)
{
    if(p_lia)
    {
        delete p_lia;
        p_lia = nullptr;
    }

    p_lia = new SITUIOLinkAssociation(link_associations);

}

int SITUIOObject::s_id()
{
    return d_s_id;
}

bool SITUIOObject::has_token_component()
{
    return p_token;
}

bool SITUIOObject::has_pointer_component()
{
    return p_pointer;
}

bool SITUIOObject::has_bounds_component()
{
    return p_bounds;
}

bool SITUIOObject::has_symbol_component()
{
    return p_symbol;
}

bool SITUIOObject::has_outer_counter_geometry_component()
{
    return p_ocg;
}

bool SITUIOObject::has_linking_association_component()
{
    return p_lia;
}

bool SITUIOObject::has_any_component()
{
    return p_token || p_pointer || p_bounds || p_symbol || p_ocg || p_lia;
}

SITUIOToken *const SITUIOObject::token_component() const
{
    return p_token;
}

SITUIOPointer *const SITUIOObject::pointer_component() const
{
    return p_pointer;
}

SITUIOBounds *const SITUIOObject::bounds_component() const
{
    return p_bounds;
}

const SITUIOSymbol *SITUIOObject::symbol_component() const
{
    return p_symbol;
}

const SITUIOOuterContourGeometry *SITUIOObject::outer_contour_geometry_component() const
{
    return p_ocg;
}

SITUIOLinkAssociation *const SITUIOObject::link_association() const
{
    return p_lia;
}
