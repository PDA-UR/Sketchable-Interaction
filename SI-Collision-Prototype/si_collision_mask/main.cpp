#include <iostream>
#include <vector>

template <typename T>
void print(T value)
{
    std::cout << value << std::endl;
}

class Point
{
public:
    Point(int x, int y) : d_x(x), d_y(y)
    {}

    int x() { return d_x; }
    int y() { return d_y; }

private:

    int d_x, d_y;
};

void print(Point& p)
{
    print("" + std::to_string(p.x()) + " " + std::to_string(p.y()));
}

class Line
{
public:
    Line(Point p, Point q) : d_p(p), d_q(q)
    {}

    Point p() { return d_p; }
    Point q() { return d_q; }

private:
    Point d_p, d_q;
};

class Mask
{
public:
    Mask(std::vector<Point> &contour)
    {
        int ymin = 9000, ymax = -1;

        std::vector<Line> edges;

        retrieve_edges(edges, contour);
        retrieve_y_minumum_maximum_of_contour(ymin, ymax, contour);
        fill_contour_scanline(ymin, ymax, edges, contour);
    }

    void set_bit(Point p)
    {
        int index = 1919 * p.y() + p.x();

        d_values[index / 32] |= 1 << (index % 32);
    }

    void clear_bit(Point p)
    {
        int index = 1919  * p.y() + p.x();

        d_values[index / 32] &= ~(1 << (index % 32));
    }

    int test_bit(Point p)
    {
        int index = 1919   * p.y() + p.x();

        return ((d_values[index / 32] & (1 << (index % 32))) != 0);
    }

private:
    Point* intersect(Line& l, Line& m)
    {
        Point p1 = l.p(), p2 = l.q(), p3 = m.p(), p4 = m.q();

        int x1 = p1.x(), x2 = p2.x(), x3 = p3.x(), x4 = p4.x();
        int y1 = p1.y(), y2 = p2.y(), y3 = p3.y(), y4 = p4.y();

        int d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (d == 0) return nullptr;

        int  pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);

        int x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
        int y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

        if ( x < std::min(x1, x2) || x > std::max(x1, x2) ||
             x < std::min(x3, x4) || x > std::max(x3, x4) ) return nullptr;
        if ( y < std::min(y1, y2) || y > std::max(y1, y2) ||
             y < std::min(y3, y4) || y > std::max(y3, y4) ) return nullptr;

        return new Point(x, y);
    }

    bool point_in_polygon(Point& point, std::vector<Point>&  points) {
        int i, j, nvert = points.size();

        bool c = false;

        for(i = 0, j = nvert - 1; i < nvert; j = i++) {
            if(((points[i].y() >= point.y() ) != (points[j].y() >= point.y()) ) &&
                (point.x() <= (points[j].x() - points[i].x()) * (point.y() - points[i].y()) / (points[j].y() - points[i].y()) + points[i].x()))
                c = !c;
        }

        return c;
    }

    const Point contour_center(std::vector<Point>& vertices)
    {
        int px = 0;
        int py = 0;

        for(auto &p : vertices)
        {
            px += p.x();
            py += p.y();
        }

        px /= vertices.size();
        py /= vertices.size();

        return Point(px, py);
    }

    void retrieve_edges(std::vector<Line>& edges, const std::vector<Point>& contour)
    {
        int i;

        for (i = 1; i < contour.size(); i++)
            edges.emplace_back(contour[i - 1], contour[i]);

        edges.emplace_back(contour[i], contour[0]);
    }

    void retrieve_y_minumum_maximum_of_contour(int& ymin, int& ymax, std::vector<Point>& contour)
    {
        for(auto& p : contour)
        {
            if(p.y() < ymin)
                ymin = p.y();
            else if(p.y() > ymax)
                ymax = p.y() + 1;
        }
    }

    void fill_contour_scanline(int ymin, int ymax, std::vector<Line>& edges, std::vector<Point>& contour)
    {
        for(int k = ymin; k < ymax; k++)
        {
            Line l(Point(0, k), Point(1079, k));

            std::vector<Point> intersections;

            for(auto& edge : edges)
            {
                Point* p = intersect(l, edge);

                if(p)
                    intersections.push_back(*p);
            }

            std::vector<std::vector<Point>> pairs_to_fill;

            for(int n = 1; n < intersections.size(); n++)
                pairs_to_fill.emplace_back(std::vector<Point> {intersections[n - 1], intersections[1]});

            for(auto& pair : pairs_to_fill)
            {
                Point middle(pair[0].x() + (pair[1].x() - pair[0].x()) / 2, pair[0].y());

                if(point_in_polygon(middle, contour))
                    for(int x = pair[0].x(); x <= pair[1].x(); x++)
                        set_bit(Point(x, pair[0].y()));
            }
        }
    }

    int d_values[1920 * 1080 / 32];
};

int main()
{
    std::vector<Point> pts =
    {
            Point(100, 100), Point(100, 600), Point(350, 375), Point(600, 600), Point(600, 100), Point(350, 325)
    };

    Mask m(pts);

    std::cout << m.test_bit(Point(350, 100)) << std::endl;

    return 0;
}