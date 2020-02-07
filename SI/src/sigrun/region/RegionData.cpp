
#include "RegionData.hpp"

RegionData::RegionData(const std::string &field, int type, const std::any &value):
    d_field(field),
    d_type(type),
    d_value(value)
{

}

RegionData::~RegionData() = default;

const std::string &RegionData::field() const
{
    return d_field;
}

const int RegionData::type() const
{
    return d_type;
}

const std::any& RegionData::value() const
{
    return d_value;
}
