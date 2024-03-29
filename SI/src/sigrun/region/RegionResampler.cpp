

#include <numeric>
#include <execution>
#include "RegionResampler.hpp"

void RegionResampler::resample(std::vector<glm::vec3> &out, const std::vector<glm::vec3> &in, int step_count)
{
    auto points = in;
    out.reserve(step_count);

    if (!points.empty())
    {
        float length = total_length(points);
        float stepsize = length / (step_count - 1);
        float current_position = 0;

        out.push_back(points[0]);

        for (uint32_t i = 1; i < points.size(); i++)
        {
            float d = distance(points[i - 1], points[i]);

            if (current_position + d >= stepsize)
            {
                float nx = points[i - 1].x + ((stepsize - current_position) / d) * (points[i].x - points[i - 1].x);
                float ny = points[i - 1].y + ((stepsize - current_position) / d) * (points[i].y - points[i - 1].y);

                glm::vec3 vert(nx, ny, 1);

                out.emplace_back(vert);
                points.insert(points.begin() + i, 1, vert);

                current_position = 0;
            }
            else
                current_position += d;
        }

        if(out.size() < step_count)
            for(int32_t i = out.size(); i < step_count; ++i)
                out.emplace_back(out.back());

        if(out.size() > step_count)
            Print::print("Outgoing contour contains more points than " + std::to_string(step_count));
    }
}

float RegionResampler::distance(const glm::vec3 &p, const glm::vec3 &q)
{
    return glm::distance(glm::vec2(p.x, p.y), glm::vec2(q.x, q.y));
}

float RegionResampler::total_length(const std::vector<glm::vec3> &vertices)
{
    if (!vertices.empty())
    {
        float d = 0.0f;
        glm::vec3 p0 = vertices[0];

        for(const auto& p: vertices)
        {
            d += distance(p0, p);
            p0 = p;
        }

        return d;
    }

    return 0.0f;
}
