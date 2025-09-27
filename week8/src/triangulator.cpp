#include "triangulator.h"
#include <iostream>
#include <algorithm>

using namespace std;

double PolygonTriangulator::crossProduct(int a, int b, int c) {
    Point ab = vertices[indices[b]] - vertices[indices[a]];
    Point ac = vertices[indices[c]] - vertices[indices[a]];
    return ab.cross(ac);
}

bool PolygonTriangulator::pointInTriangle(const Point& p, int a, int b, int c) {
    Point pa = vertices[indices[a]] - p;
    Point pb = vertices[indices[b]] - p;
    Point pc = vertices[indices[c]] - p;
    
    double c1 = pa.cross(pb);
    double c2 = pb.cross(pc);
    double c3 = pc.cross(pa);
    
    return (c1 >= 0 && c2 >= 0 && c3 >= 0) || (c1 <= 0 && c2 <= 0 && c3 <= 0);
}

bool PolygonTriangulator::isEar(int prev, int curr, int next) {
    if (crossProduct(prev, curr, next) <= 0) {
        return false;
    }
    
    for (size_t i = 0; i < indices.size(); i++) {
        if ((int)i == prev || (int)i == curr || (int)i == next) {
            continue;
        }
        
        if (pointInTriangle(vertices[indices[i]], prev, curr, next)) {
            return false;
        }
    }
    
    return true;
}

int PolygonTriangulator::getPrev(int i) {
    return (i - 1 + indices.size()) % indices.size();
}

int PolygonTriangulator::getNext(int i) {
    return (i + 1) % indices.size();
}

void PolygonTriangulator::setVertices(const vector<Point>& verts) {
    vertices = verts;
    indices.clear();
    triangles.clear();
    
    for (size_t i = 0; i < vertices.size(); i++) {
        indices.push_back((int)i);
    }
}

vector<Triangle> PolygonTriangulator::triangulate() {
    triangles.clear();
    
    if (vertices.size() < 3) {
        return triangles;
    }
    
    while (indices.size() > 2) {
        bool earFound = false;
        
        for (size_t i = 0; i < indices.size(); i++) {
            int prev = getPrev((int)i);
            int next = getNext((int)i);
            
            if (isEar(prev, (int)i, next)) {
                triangles.push_back(Triangle(indices[prev], indices[i], indices[next]));
                
                indices.erase(indices.begin() + i);
                earFound = true;
                break;
            }
        }
        
        if (!earFound) {
            cout << "Error: No ear found. Polygon might be invalid." << endl;
            break;
        }
    }
    
    return triangles;
}

void PolygonTriangulator::printTriangles() {
    cout << "Triangulation result:" << endl;
    for (size_t i = 0; i < triangles.size(); i++) {
        const Triangle& t = triangles[i];
        cout << "Triangle " << i + 1 << ": (" 
             << vertices[t.a].x << "," << vertices[t.a].y << ") ("
             << vertices[t.b].x << "," << vertices[t.b].y << ") ("
             << vertices[t.c].x << "," << vertices[t.c].y << ")" << endl;
    }
}

const vector<Point>& PolygonTriangulator::getVertices() const {
    return vertices;
}