#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    bool operator==(const Point& other) const {
        const double EPS = 1e-9;
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }
};

enum PointLocation {
    INSIDE,
    OUTSIDE,
    ON_BOUNDARY
};

double cross(const Point& a, const Point& b);

bool onSegment(const Point& a, const Point& b, const Point& c);

bool compareByX(const Point& a, const Point& b);

class ConvexPolygon {
private:
    vector<Point> upperChain;
    vector<Point> lowerChain;
    Point leftmost, rightmost;
    
public:
    void buildFromVertices(const vector<Point>& vertices);
    PointLocation queryPoint(const Point& q) const;
    
    const vector<Point>& getUpperChain() const { return upperChain; }
    const vector<Point>& getLowerChain() const { return lowerChain; }
};

vector<Point> readPolygon(const string& filename);
vector<Point> readPoints(const string& filename);
void printResults(const vector<Point>& points, const vector<PointLocation>& results);

vector<PointLocation> processQueries(const ConvexPolygon& polygon, const vector<Point>& queryPoints);
void printPolygonChains(const ConvexPolygon& polygon);

#endif
