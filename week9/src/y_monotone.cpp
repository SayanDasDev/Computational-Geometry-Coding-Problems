#include "y_monotone.h"
#include <algorithm>
#include <stack>
#include <iostream>
#include <cmath>
#include <utility>

using namespace std;

YMonotoneTriangulator::YMonotoneTriangulator(const vector<Point>& points) : polygon(points) {}

bool YMonotoneTriangulator::isYMonotone(const vector<Point>& poly) {
    int n = poly.size();
    if (n < 3) return false;
    
    int topmost = 0, bottommost = 0;
    for (int i = 1; i < n; i++) {
        if (poly[i].y > poly[topmost].y || (poly[i].y == poly[topmost].y && poly[i].x < poly[topmost].x)) {
            topmost = i;
        }
        if (poly[i].y < poly[bottommost].y || (poly[i].y == poly[bottommost].y && poly[i].x < poly[bottommost].x)) {
            bottommost = i;
        }
    }
    
    return true;
}

void YMonotoneTriangulator::triangulateYMonotone(const vector<Point>& poly) {
    int n = poly.size();
    if (n < 3) return;
    
    if (n == 3) {
        triangles.push_back(Triangle(0, 1, 2));
        return;
    }
    
    if (n == 4) {
        triangles.push_back(Triangle(0, 1, 2));
        triangles.push_back(Triangle(0, 2, 3));
        return;
    }
    
    for (int i = 1; i < n - 1; i++) {
        triangles.push_back(Triangle(0, i, i + 1));
    }
}

vector<Triangle> YMonotoneTriangulator::triangulate() {
    triangles.clear();
    
    if (!isYMonotone(polygon)) {
        cout << "Warning: Polygon may not be y-monotone" << endl;
    }
    
    triangulateYMonotone(polygon);
    return triangles;
}

void YMonotoneTriangulator::printTriangulation() {
    OutputWriter::writeTriangles(triangles, polygon);
}