#ifndef Y_MONOTONE_H
#define Y_MONOTONE_H

#include <vector>
#include "point.h"
#include "io.h"

using namespace std;

enum VertexType {
    START,
    END,
    REGULAR,
    SPLIT,
    MERGE
};

struct Vertex {
    Point point;
    VertexType type;
    int index;
    
    Vertex(const Point& p, int idx) : point(p), index(idx) {}
};

class YMonotoneTriangulator {
private:
    vector<Point> polygon;
    vector<Triangle> triangles;
    
    VertexType classifyVertex(int i, const vector<Point>& sortedPolygon);
    bool isYMonotone(const vector<Point>& poly);
    void triangulateYMonotone(const vector<Point>& poly);
    
public:
    YMonotoneTriangulator(const vector<Point>& points);
    vector<Triangle> triangulate();
    void printTriangulation();
};

#endif