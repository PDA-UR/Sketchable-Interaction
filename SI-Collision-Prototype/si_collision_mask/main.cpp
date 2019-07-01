#include <iostream>
#include <vector>
#include <algorithm>

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

bool point_in_polygon(Point& point, std::vector<Point>&  points)
{
    int i, j, nvert = points.size();

    bool c = false;

    for(i = 0, j = nvert - 1; i < nvert; j = i++) {
        if(((points[i].y() >= point.y() ) != (points[j].y() >= point.y()) ) &&
           (point.x() <= (points[j].x() - points[i].x()) * (point.y() - points[i].y()) / (points[j].y() - points[i].y()) + points[i].x()))
            c = !c;
    }

    return c;
}

class Mask
{
public:
    Mask(std::vector<Point> &contour)
    {
        int ymin = 9000, ymax = -1;

        std::vector<Line> edges;

        for(int& d_value : d_values)
            d_value = 0;

        retrieve_edges(edges, contour);
        retrieve_y_minumum_maximum_of_contour(ymin, ymax, contour);
        fill_contour_scanline(ymin, ymax, edges, contour);
    }

    void set_bit(Point p)
    {
        int index = 1920 * p.y() + p.x();

        d_values[index / 32] |= 1 << (index % 32);
    }

    void clear_bit(Point p)
    {
        int index = 1920  * p.y() + p.x();

        d_values[index / 32] &= ~(1 << (index % 32));
    }

    int test_bit(Point p)
    {
        int index = 1920 * p.y() + p.x();

        return ((d_values[index / 32] & (1 << (index % 32))) != 0);
    }

private:
    Point* intersect(Line& l, Line& m)
    {
        float epsilon = 0.1;

        Point p1 = l.p(), p2 = l.q(), p3 = m.p(), p4 = m.q();

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

        return new Point((int) x, (int) y);
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
        for(int k = ymax; k > ymin; k--)
        {
            Line l(Point(0, k), Point(1080, k));

            std::vector<Point> intersections;

            for(auto& edge : edges)
            {
                Point* p = intersect(l, edge);

                if(p)
                    intersections.push_back(*p);
            }

            std::sort(intersections.begin(), intersections.end(), [&](Point& p, Point& q)
            {
                return p.x() < q.x();
            });

            //for (auto& p : intersections)
                //std::cout << p.x() << " " << p.y() << " | ";

            //std::cout << std::endl;

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
        Point(100, 100),
        Point(100, 600),
        Point(350, 375),
        Point(600, 600),
        Point(600, 100),
        Point(350, 325),
        Point(100, 100)
    };

    Mask m(pts);

    Point p(350, 350);

    print((m.test_bit(p) ? "Mask: In Polygon" : "Mask: Not in Polygon"));
    print(point_in_polygon(p, pts) ? "Conventional: In Polygon" : "Conventional: Not in Polygon");

    return 0;
}