#include "Dollar1GestureRecognizer.hpp"

std::vector<glm::vec3> $1Helper::$1_rotate_to_zero(const std::vector<glm::vec3>& points)
{
    auto c = $1Helper::$1_centroid(points);
    float theta = std::atan2(c.y - points[0].y, c.x - points[0].x);
    return $1Helper::$1_rotate_by(points, -theta);
}

std::vector<glm::vec3> $1Helper::$1_rotate_by(const std::vector<glm::vec3>& points, float theta)
{
    auto c = $1Helper::$1_centroid(points);
    float cos = std::cos(theta);
    float sin = std::sin(theta);

    std::vector<glm::vec3> new_points;

    for(auto& p: points)
    {
        float qx = (p.x - c.x) * cos - (p.y - c.y) * sin + c.y;
        float qy = (p.x - c.x) * sin + (p.y - c.y) * cos + c.y;

        new_points.emplace_back(qx, qy, 1);
    }

    return new_points;
}

std::vector<glm::vec3> $1Helper::$1_scale_to_square(const std::vector<glm::vec3>& points, float size)
{
    auto b = $1Helper::bounding_box(points);

    std::vector<glm::vec3> new_points;

    for(auto& p: points)
    {
        float qx = p.x * (size / b[1].x);
        float qy = p.y * (size / b[1].y);

        new_points.emplace_back(qx, qy, 1);
    }

    return new_points;
}

std::vector<glm::vec3> $1Helper::$1_translate_to_origin(const std::vector<glm::vec3>& points)
{
    auto c = $1Helper::$1_centroid(points);

    std::vector<glm::vec3> new_points;

    for(auto& p: points)
    {
        float qx = p.x - c.x;
        float qy = p.y - c.y;

        new_points.emplace_back(qx, qy, 1);
    }

    return new_points;
}

float $1Helper::$1_distance_at_best_angle(const std::vector<glm::vec3>& points, const Template& t, float a, float b, float threshold)
{
    float x1 = $1_PHI * a + (1.0 - $1_PHI) * b;
    float f1 = $1Helper::$1_distance_at_angle(points, t, x1);
    float x2 = (1.0 - $1_PHI) * a + $1_PHI * b;
    float f2 = $1Helper::$1_distance_at_angle(points, t, x2);

    while(std::abs(b - a) > threshold)
    {
        if(f1 < f2)
        {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = $1_PHI * a + (1.0 - $1_PHI) * b;
            f1 = $1Helper::$1_distance_at_angle(points, t, x1);
        }
        else
        {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = (1.0 - $1_PHI) * a + $1_PHI * b;
            f2 = $1Helper::$1_distance_at_angle(points, t, x2);
        }
    }

    return std::min(f1, f2);
}

float $1Helper::$1_distance_at_angle(const std::vector<glm::vec3>& points, const Template& t, float theta)
{
    std::vector<glm::vec3> new_points = $1Helper::$1_rotate_by(points, theta);
    return $1Helper::path_distance(new_points, t.points());
}

glm::vec3 $1Helper::$1_centroid(const std::vector<glm::vec3>& points)
{
    float x = 0.0;
    float y = 0.0;

    for(auto& p: points)
    {
        x += p.x;
        y += p.y;
    }

    x /= points.size();
    y /= points.size();

    return glm::vec3(x, y, 1);
}

std::vector<glm::vec3> $1Helper::bounding_box(const std::vector<glm::vec3>& points)
{
    float min_x = 9999999;
    float min_y = 9999999;
    float max_x = -9999999;
    float max_y = -9999999;

    for(auto& p: points)
    {
        if(p.x < min_x)
            min_x = p.x;

        if(p.x > max_x)
            max_x = p.x;

        if(p.y < min_y)
            min_y = p.y;

        if(p.y > max_y)
            max_y = p.y;
    }

    return std::vector<glm::vec3> {glm::vec3(min_x, min_y, 1), glm::vec3(max_x - min_x, max_y - min_y, 1)};
}

float $1Helper::path_distance(const std::vector<glm::vec3>& pts1, const std::vector<glm::vec3>& pts2)
{
    float d = 0.0;

    for(int32_t i = 0; i < pts1.size(); ++i)
        d += $1Helper::distance(pts1[i], pts2[i]);

    return d / pts1.size();
}

float $1Helper::distance(const glm::vec3& a, const glm::vec3& b)
{
    glm::vec3 dp = b - a;
    return std::sqrt(dp.x * dp.x + dp.y * dp.y);
}

Template::Template(const std::string &name, const std::vector<glm::vec3> &points):
    d_name(name)
{
    RegionResampler::resample(d_points, points, 64);
    d_points = $1Helper::$1_rotate_to_zero(d_points);
    d_points = $1Helper::$1_scale_to_square(d_points, $1_SQUARE_SIZE);
    d_points = $1Helper::$1_translate_to_origin(d_points);
}

const std::vector<glm::vec3>& Template::points() const
{
    return d_points;
}

const std::string& Template::name() const
{
    return d_name;
}

Recognizer::Recognizer()
{
    // clockwise horizontal
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(300, 0, 1),
        glm::vec3(300, 150, 1),
        glm::vec3(0, 150, 1),
    });

    // counter-clockwise horizontal
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 150, 1),
        glm::vec3(300, 150, 1),
        glm::vec3(300, 0, 1),
    });

    // clockwise vertical
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(150, 0, 1),
        glm::vec3(150, 300, 1),
        glm::vec3(0, 300, 1),
    });

    // counter-clockwise vertical
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 300, 1),
        glm::vec3(150, 300, 1),
        glm::vec3(150, 0, 1),
    });

    // counter-clockwise square vertical
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(150, 0, 1),
        glm::vec3(150, 150, 1),
        glm::vec3(0, 150, 1),
    });

    // counter-clockwise square horizontal
    d_templates.emplace_back("Rectangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(0, 150, 1),
        glm::vec3(150, 150, 1),
        glm::vec3(150, 0, 1),
    });

    // clockwise
    d_templates.emplace_back("Triangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(255, 0, 1),
        glm::vec3(255 / 2, std::sqrt(3) / 2 * 255, 1),
    });

    // counter-clockwise
    d_templates.emplace_back("Triangle", std::vector<glm::vec3>
    {
        glm::vec3(0, 0, 1),
        glm::vec3(255 / 2, std::sqrt(3) / 2 * 255, 1),
        glm::vec3(255, 0, 1),
    });

    // clockwise
    float cx = 100;
    float cy = 100;
    float radius = 50;
    std::vector<glm::vec3> perfect_circle;

    for(int i = 0; i < 360; ++i)
        perfect_circle.emplace_back(radius * std::cos(i * M_PI / 180) + cx, radius * std::sin(i * M_PI / 180) + cy, 1);

    d_templates.emplace_back("Circle", perfect_circle);

    // counter-clockwise
    perfect_circle.clear();
    for(int i = 359; i >= 0; --i)
        perfect_circle.emplace_back(radius * std::cos(i * M_PI / 180) + cx, radius * std::sin(i * M_PI / 180) + cy, 1);

    d_templates.emplace_back("Circle", perfect_circle);
}

float Recognizer::recognize(std::vector<glm::vec3> &out, const std::vector<glm::vec3> &in)
{
    std::vector<glm::vec3> points;
    auto bb = $1Helper::bounding_box(in);

    RegionResampler::resample(points, in, 64);
    points = $1Helper::$1_rotate_to_zero(points);
    points = $1Helper::$1_scale_to_square(points, $1_SQUARE_SIZE);
    points = $1Helper::$1_translate_to_origin(points);

    float b = 9999999.0;

    std::string temp_name = "";
    for(auto& temp: d_templates)
    {
        float d = $1Helper::$1_distance_at_best_angle(points, temp, -$1_ANGLE_RANGE, $1_ANGLE_RANGE, $1_ANGLE_PRECISION);

        if(d < b)
        {
            b = d;
            temp_name = temp.name();
        }
    }

    Result r(temp_name, 1.0 - ((float) b / (float) $1_HALF_DIAGONAL));

    if(r.score() > 0.95f)
    {
        if(r.name() == "Triangle")
        {
            INFO("Shape approximates to a triangle. Triangle generated in its place!");

            out = std::vector<glm::vec3>
            {
                glm::vec3(bb[0].x, bb[0].y + bb[1].y, 1),
                glm::vec3(bb[0].x + bb[1].x / 2, bb[0].y, 1),
                glm::vec3(bb[0].x + bb[1].x, bb[0].y + bb[1].y, 1)
            };
        }
        else if(r.name() == "Rectangle")
        {
            INFO("Shape approximates to a rectangle. Rectangle generated in its place!");

            out = std::vector<glm::vec3>
            {
                glm::vec3(bb[0].x, bb[0].y, 1),
                glm::vec3(bb[0].x, bb[0].y + bb[1].y, 1),
                glm::vec3(bb[0].x + bb[1].x, bb[0].y + bb[1].y, 1),
                glm::vec3(bb[0].x + bb[1].x, bb[0].y, 1)
            };
        }
        else if(r.name() == "Circle")
        {
            INFO("Shape approximates to a circle. Circle generated in its place!");

            float cx = bb[0].x + bb[1].x / 2;
            float cy = bb[0].y + bb[1].y / 2;
            float radius = bb[1].x / 2;

            for(int i = 0; i < 360; ++i)
                out.emplace_back(radius * std::cos(i * M_PI / 180) + cx, radius * std::sin(i * M_PI / 180) + cy, 1);
        }
    }
    else
    {
        out = in;
    }

    return r.score();
}

const std::vector<Template>& Recognizer::templates() const
{
    return d_templates;
}
