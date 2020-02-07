
#ifndef SITEST_REGIONDATAQUEUE_HPP
#define SITEST_REGIONDATAQUEUE_HPP

#include <memory>
#include <any>

#define BOOL 0
#define FLOAT 1
#define INT 2
#define STRING 3
#define LONG 4
#define DOUBLE 5

class RegionData
{
public:
    enum TYPE
    {
        _BOOL = BOOL,
        _FLOAT = FLOAT,
        _INT = INT,
        _STRING = STRING,
        _LONG = LONG,
        _DOUBLE = DOUBLE
    };

    RegionData(const std::string& field, int type, const std::any& value);
    ~RegionData();

    const std::string& field() const;
    const int type() const;
    const std::any& value() const;

private:
    std::string d_field;
    int d_type;
    std::any d_value;
};


#endif //SITEST_REGIONDATAQUEUE_HPP
