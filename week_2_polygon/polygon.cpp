#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

const double EPS = 1e-6;

struct Point
{
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    Point operator-(const Point &other) const
    {
        return Point(x - other.x, y - other.y);
    }

    bool operator==(const Point &other) const
    {

        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }
};

double crossProduct(const Point &u, const Point &v)
{
    return u.x * v.y - u.y * v.x;
}

int orientation(const Point &p, const Point &q, const Point &r)
{
    double cross = crossProduct(q - p, r - q);

    if (abs(cross) < EPS)
        return 0;
    return (cross > 0) ? 2 : 1;
}

bool onSegment(const Point &p, const Point &q, const Point &r)
{
    return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
           q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y);
}

bool doIntersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    return false;
}

bool isSimplePolygon(const vector<Point> &points)
{
    int n = points.size();
    if (n < 3)
        return false;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 2; j < n; j++)
        {
            if (i == 0 && j == n - 1)
                continue;

            Point p1 = points[i];
            Point q1 = points[(i + 1) % n];
            Point p2 = points[j];
            Point q2 = points[(j + 1) % n];

            if (doIntersect(p1, q1, p2, q2))
            {
                return false;
            }
        }
    }
    return true;
}

bool isConvexPolygon(const vector<Point> &points)
{
    int n = points.size();
    if (n < 3)
        return false;

    bool hasPositive = false, hasNegative = false;

    for (int i = 0; i < n; i++)
    {
        Point p1 = points[i];
        Point p2 = points[(i + 1) % n];
        Point p3 = points[(i + 2) % n];

        double cross = crossProduct(p2 - p1, p3 - p2);

        if (abs(cross) > EPS)
        {
            if (cross > 0)
                hasPositive = true;
            if (cross < 0)
                hasNegative = true;
        }

        if (hasPositive && hasNegative)
            return false;
    }

    return true;
}

vector<Point> removeDuplicates(const vector<Point> &points)
{
    vector<Point> result;
    for (const Point &p : points)
    {
        if (result.empty() || !(result.back() == p))
        {
            result.push_back(p);
        }
    }

    if (result.size() > 1 && result[0] == result.back())
    {
        result.pop_back();
    }

    return result;
}

int main(int argc, char* argv[])
{
    string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        cout << "Enter filename: ";
        cin >> filename;
    }

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << endl;
        return 1;
    }

    vector<Point> points;
    double x, y;

    while (file >> x >> y)
    {
        points.push_back(Point(x, y));
    }
    file.close();

    cout << "Read " << points.size() << " points from file." << endl;

    if (points.size() < 3)
    {
        cout << "Error: Need at least 3 points to form a polygon." << endl;
        return 1;
    }

    points = removeDuplicates(points);
    cout << "After removing duplicates: " << points.size() << " points." << endl;

    if (points.size() < 3)
    {
        cout << "Error: After removing duplicates, less than 3 points remain." << endl;
        return 1;
    }

    bool simple = isSimplePolygon(points);
    cout << "\nIs Simple Polygon: " << (simple ? "YES" : "NO") << endl;

    bool convex = isConvexPolygon(points);
    cout << "Is Convex Polygon: " << (convex ? "YES" : "NO") << endl;

    if (!simple)
    {
        cout << "\nThe polygon is not a simple polygon." << endl;
    }
    else if (!convex)
    {
        cout << "\nThe polygon is simple but not-convex (concave)." << endl;
    }
    else
    {
        cout << "\nThe polygon is both simple and convex." << endl;
    }

    return 0;
}