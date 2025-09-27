#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include <vector>
#include "point.h"
#include "triangle.h"

using namespace std;

class PolygonTriangulator {
private:
    vector<Point> vertices;
    vector<int> indices;
    vector<Triangle> triangles;
    
    double crossProduct(int a, int b, int c);
    bool pointInTriangle(const Point& p, int a, int b, int c);
    bool isEar(int prev, int curr, int next);
    int getPrev(int i);
    int getNext(int i);
    
public:
    void setVertices(const vector<Point>& verts);
    vector<Triangle> triangulate();
    void printTriangles();
    const vector<Point>& getVertices() const;
};

#endif