#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>

#define WIDTH 1920
#define HEIGHT 1080

class Vector
{
public:
    Vector(int x, int y) : d_x(x), d_y(y)
    {}

    const int x() const { return d_x; }
    const int y() const { return d_y; }

private:

    int d_x, d_y;
};

void print(Vector& p)
{
    std::cout << "" + std::to_string(p.x()) + " " + std::to_string(p.y()) << std::endl;
}

template <int Width, int Height>
class Mask : public std::bitset<Width * Height>
{
public:
    explicit Mask(const std::vector<Vector>& contour) : d_width(Width), d_height(Height)
    {
        std::vector<std::vector<Vector>> edges;
        std::vector<Vector> c = contour;
        c.push_back(c[0]);

        int ymin = std::min_element(contour.begin(), contour.end(), [&](const Vector& v, const Vector& u) { return v.y() < u.y(); }).base()->y();
        int ymax = std::max_element(contour.begin(), contour.end(), [&](const Vector& v, const Vector& u) { return v.y() < u.y(); }).base()->y();

        retrieve_edges(edges, c);
        fill_contour_scanline(ymin, ymax, edges, c);
    };

    ~Mask() = default;

    void set_bit(const Vector& v)
    {
        this->set(d_width * v.y() + v.x(), true);
    }

    void clear_bit(const Vector& v)
    {
        this->set(d_width * v.y() + v.x(), false);
    }

    bool test_bit(const Vector& v)
    {
        return this->test(d_width * v.y() + v.x());
    }

    const int width() const
    {
        return d_width;
    }

    const int height() const
    {
        return d_height;
    }

private:
    const Vector* intersect(const std::vector<Vector>& l, const std::vector<Vector>& m)
    {
        float epsilon = 0.0001;

        const Vector& p1 = l[0], & p2 = l[1], & p3 = m[0], & p4 = m[1];

        float x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
        float y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

        float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (d == 0)
            return nullptr;

        float pre = (x1 * y2 - y1 * x2);
        float post = (x3 * y4 - y3 * x4);

        float x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
        float y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

        if (x < (std::min(x1, x2) - epsilon) || x > (std::max(x1, x2) + epsilon) ||  x < (std::min(x3, x4) - epsilon) || x > (std::max(x3, x4) + epsilon))
            return nullptr;

        if (y < (std::min(y1, y2) - epsilon) || y > (std::max(y1, y2) + epsilon) || y < (std::min(y3, y4) - epsilon)|| y > (std::max(y3, y4) + epsilon))
            return nullptr;

        return new Vector((int) x, (int) y);
    }

    void retrieve_edges(std::vector<std::vector<Vector>>& dest, const std::vector<Vector>& contour)
    {
        int i;

        for(i = 1; i < contour.size(); i++)
            dest.emplace_back(std::vector<Vector> {contour[i - 1], contour[i]});

        dest.emplace_back(std::vector<Vector> {contour[i], contour[0]});
    }

    void fill_contour_scanline(int ymin, int ymax, const std::vector<std::vector<Vector>>& edges, const std::vector<Vector>& contour)
    {
        for(int k = ymax; k > ymin; k--)
        {
            std::vector<Vector> intersections;
            std::vector<std::vector<Vector>> pairs_to_fill;
            std::vector<Vector> scanline { Vector(0, k), Vector(d_width, k) };

            for(const auto& edge : edges)
            {
                const Vector* v = intersect(scanline, edge);

                if(v)
                    intersections.push_back(*v);
            }

            std::sort(intersections.begin(), intersections.end(), [&](const Vector& v, const Vector& u) { return v.x() < u.x(); });

            for(int n = 1; n < intersections.size(); n += 2)
                pairs_to_fill.emplace_back(std::vector<Vector> { intersections[n - 1], intersections[n] });

            for(auto& pair : pairs_to_fill)
                for (int x = pair[0].x(); x <= pair[1].x(); x++)
                    set_bit(Vector(x, pair[1].y()));
        }
    }

    int d_width, d_height;
};

void debug_print_mask(const Mask<WIDTH, HEIGHT>& m)
{
    for(int i = 0; i < m.height(); i++)
    {
        for (int k = 0; k < m.width(); k++)
            std::cout << m[m.width() * i + k];

        std::cout << std::endl;
    }
}

int main()
{
    std::vector<Vector> contour =
    {
        Vector(100, 100),
        Vector(100, 600),
        Vector(350, 375),
        Vector(600, 600),
        Vector(600, 100),
        Vector(350, 325),
    };

    Mask<WIDTH, HEIGHT> m(contour);

    debug_print_mask(m);

    return 0;
}