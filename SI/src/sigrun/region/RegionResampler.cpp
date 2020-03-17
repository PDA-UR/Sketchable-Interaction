

#include <numeric>
#include "RegionResampler.hpp"

void RegionResampler::resample(std::vector<glm::vec3> &out, const std::vector<glm::vec3> &in, int step_count)
{
    auto points = in;

    if (!points.empty())
    {
        float length = total_length(points);
        float stepsize = length / (step_count - 1);
        float current_position = 0;

        out.push_back(points[0]);

        for (int i = 1; i < points.size(); i++)
        {

            auto& p0 = points[i - 1];
            float d = distance(p0, points[i]);

            if (current_position + d >= stepsize)
            {
                float nx = p0.x + ((stepsize - current_position) / d) * (points[i].x - p0.x);
                float ny = p0.y + ((stepsize - current_position) / d) * (points[i].y - p0.y);

                glm::vec3 vert(nx, ny, 1);

                out.emplace_back(vert);
                points.insert(points.begin() + i, 1, vert);

                current_position = 0;
            }
            else
                current_position += d;
        }
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
        glm::vec3 p0 = vertices[0];
        float length = 0.0f;

        for (int i = 1; i < vertices.size(); i++)
        {
            length += distance(p0, vertices[i]);
            p0 = vertices[i];
        }

        return length;
    }

    return 0.0f;
}
