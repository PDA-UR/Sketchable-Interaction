
#include <debug/Print.hpp>
#include "RegionMask.hpp"

RegionMask::RegionMask(int canvas_width, int canvas_height, const std::vector<glm::vec3> &contour,
                       const std::vector<glm::vec3> &aabb):
    d_canvas_width(canvas_width),
    d_canvas_height(canvas_height),
    d_tlc_aabb_x(aabb[0].x),
    d_tlc_aabb_y(aabb[0].y),
    d_brc_aabb_x(aabb[2].x),
    d_brc_aabb_y(aabb[2].y),
    d_width_aabb(d_brc_aabb_x - d_tlc_aabb_x),
    d_height_aabb(d_brc_aabb_y - d_tlc_aabb_y),
    d_values(std::vector<bool>(d_width_aabb * d_height_aabb))
{
    scanlinefill(contour, aabb);

//    for(int i = 0; i < d_width_aabb; ++i)
//    {
//        for(int k = 0; k < d_height_aabb; ++k)
//        {
//            std::cout << (int) d_values[d_width_aabb * k + i];
//        }
//
//        std::cout << std::endl;
//    }
}

RegionMask::~RegionMask()
{

}

int RegionMask::size() const
{
    return d_values.size();
}

void RegionMask::set_bit(int i)
{
    if(i > -1 && i < d_values.size())
        d_values[i] = true;
}

void RegionMask::set_bit(const glm::vec3& v)
{
    int index = d_width_aabb * ((int) v.y - d_tlc_aabb_y) + (int) v.x - d_tlc_aabb_x;

    if(index > -1 && index < d_values.size())
        this->set_bit(index);
}

void RegionMask::clear_bit(int i)
{
    if(i > -1 && i < d_values.size())
        d_values[i] = false;
}

void RegionMask::clear_bit(const glm::vec3 &v)
{
    int index = d_width_aabb * ((int) v.y - d_tlc_aabb_y) + (int) v.x - d_tlc_aabb_x;

    if(index > -1 && index < d_values.size())
        this->clear_bit(index);
}

int RegionMask::width() const
{
    return d_width_aabb;
}

int RegionMask::height() const
{
    return d_height_aabb;
}

bool RegionMask::operator[](int i) const
{
    if(i > -1 && i < d_values.size())
        return d_values[i];

    return false;
}

bool RegionMask::operator[](const glm::vec3 &v) const
{
    int index = d_width_aabb * ((int) v.y - d_tlc_aabb_y) + (int) v.x - d_tlc_aabb_x;

    if(index > -1 && index < d_values.size())
        return d_values[index];

    return false;
}

void RegionMask::move(const glm::vec2& v)
{
    d_tlc_aabb_x += v.x;
    d_tlc_aabb_y += v.y;
    d_brc_aabb_x += v.x;
    d_brc_aabb_y += v.y;
}

void RegionMask::scanlinefill(const std::vector<glm::vec3>& contour, const std::vector<glm::vec3>& aabb)
{
    std::vector<glm::vec2> scan_hits;
    static glm::vec2 min, max;
    unsigned int polysize = contour.size();

    int gap = 1;
    double y;

    min.x = d_tlc_aabb_x;
    min.y = d_tlc_aabb_y;

    max.x = d_brc_aabb_x;
    max.y = d_brc_aabb_y;

    for(auto& p : contour)
    {
        if (p.x < min.x)
            min.x = p.x;

        if (p.y < min.y)
            min.y = p.y;

        if (p.x > max.x)
            max.x = p.x;

        if (p.y > max.y)
            max.y = p.y;
    }

    // Bounds check
    if ((max.x < 0) || (min.x > d_canvas_width) || (max.y < 0) || (min.y > d_canvas_height))
        return;

    // Vertically clip
    if (min.y < d_tlc_aabb_y)
        min.y = d_tlc_aabb_y;

    if (max.y > d_brc_aabb_y)
        max.y = d_brc_aabb_y;

    // start on the outside of the object we step out by 1.
    min.x -= 1;
    max.x += 1;

    // Initialise starting conditions
    y = min.y;

    // Go through each scan line iteratively, jumping by 'gap' pixels each time
    while (y < max.y)
    {
        scan_hits.clear();

        int jump = 1;
        glm::vec2 fp(contour[0].x, contour[0].y);

        for (size_t i = 0; i < polysize - 1; i++)
        {
            glm::vec2 pa(contour[i].x, contour[i].y);
            glm::vec2 pb(contour[i + 1].x, contour[i + 1].y);

            // jump double/dud points
            if (pa.x == pb.x && pa.y == pb.y)
                continue;

            // if we encounter our hull/poly start point, then we've now created the
            // closed
            // hull, jump the next segment and reset the first-point
            if ((!jump) && (fp.x == pb.x) && (fp.y == pb.y))
            {
                if (i < polysize - 2)
                {
                    fp.x = contour[i + 2].x;
                    fp.y = contour[i + 2].y;
                    jump = 1;
                    i++;
                }
            }
            else
                jump = 0;

            // test to see if this segment makes the scan-cut.
            if ((pa.y > pb.y && y < pa.y && y > pb.y) || (pa.y < pb.y && y > pa.y && y < pb.y))
            {
                glm::vec2 intersect;

                intersect.y = y;

                if (pa.x == pb.x)
                    intersect.x = pa.x;
                else
                    intersect.x = (pb.x - pa.x) / (pb.y - pa.y) * (y - pa.y) + pa.x;

                scan_hits.push_back(intersect);
            }
        }

        // Sort the scan hits by X, so we have a proper left->right ordering
        sort(scan_hits.begin(), scan_hits.end(), [&](glm::vec2 const &a, glm::vec2 const &b)
        {
            return a.x < b.x;
        });

        // generate the line segments.
        for (int i = 0, l = scan_hits.size() - 1; i < l; i += 2)
            for(int x = scan_hits[i].x; x < scan_hits[i + 1].x; x++)
                this->set_bit(glm::vec3(x, y, 1));

        y += gap;
    }

    scan_hits.clear();
}
