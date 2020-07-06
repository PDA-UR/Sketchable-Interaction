

#ifndef SITEST_DOLLAR1GESTURERECOGNIZER_HPP
#define SITEST_DOLLAR1GESTURERECOGNIZER_HPP

#include <glm/glm.hpp>
#include <vector>
#include <vector>
#include <string>
#include <algorithm>
#include <sigrun/region/RegionResampler.hpp>
#include <cmath>
#include <debug/Print.hpp>
#include <sigrun/SIObject.hpp>

#define $1_SQUARE_SIZE 250.0
#define $1_HALF_DIAGONAL (0.5 * std::sqrt($1_SQUARE_SIZE * $1_SQUARE_SIZE + $1_SQUARE_SIZE * $1_SQUARE_SIZE))
#define $1_ANGLE_RANGE 45.0
#define $1_ANGLE_PRECISION 2.0
#define $1_PHI (0.5 * (-1.0 + std::sqrt(5.0))) // Golden Ratio

class Template
{
public:
    Template(const std::string &name, const std::vector<glm::vec3> &points);

    const std::vector<glm::vec3>& points() const;
    const std::string& name() const;

private:
    std::vector<glm::vec3> d_points;
    std::string d_name;
};

class $1Helper
{
public:
    static std::vector<glm::vec3> $1_rotate_to_zero(const std::vector<glm::vec3>& points);
    static std::vector<glm::vec3> $1_rotate_by(const std::vector<glm::vec3>& points, float theta);
    static std::vector<glm::vec3> $1_scale_to_square(const std::vector<glm::vec3>& points, float size);
    static std::vector<glm::vec3> $1_translate_to_origin(const std::vector<glm::vec3>& points);
    static float $1_distance_at_best_angle(const std::vector<glm::vec3>& points, const Template& t, float a, float b, float threshold);
    static float $1_distance_at_angle(const std::vector<glm::vec3>& points, const Template& t, float theta);
    static glm::vec3 $1_centroid(const std::vector<glm::vec3>& points);
    static std::vector<glm::vec3> bounding_box(const std::vector<glm::vec3>& points);
    static float path_distance(const std::vector<glm::vec3>& pts1, const std::vector<glm::vec3>& pts2);
    static float distance(const glm::vec3& a, const glm::vec3& b);
};

class Result
{
public:
    Result(const std::string& name, float score):
        d_name(name), d_score(score)
    {}

    const std::string& name() const
    {
        return d_name;
    }

    float score() const
    {
        return d_score;
    }

private:
    std::string d_name;
    float d_score;

};

class Recognizer: public SIObject
{ SIGRUN
public:
    Recognizer();

    float recognize(std::vector<glm::vec3>& out, const std::vector<glm::vec3>& in);
    const std::vector<Template>& templates() const;

private:
    std::vector<Template> d_templates;
};

#endif //SITEST_DOLLAR1GESTURERECOGNIZER_HPP
