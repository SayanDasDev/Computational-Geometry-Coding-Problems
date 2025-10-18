#include "io.h"
#include <iostream>
#include <fstream>

using namespace std;

vector<Point> FileReader::readPointsFromFile(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return points;
    }
    
    double x, y;
    int index = 0;
    while (file >> x >> y) {
        points.push_back(Point(x, y, index++));
    }
    
    file.close();
    cout << "Read " << points.size() << " points from " << filename << endl;
    
    return points;
}

void OutputWriter::writeTriangles(const vector<Triangle>& triangles, const vector<Point>& points) {
    cout << "Triangulation of y-monotone polygon:" << endl;
    cout << "Number of triangles: " << triangles.size() << endl;
    
    for (size_t i = 0; i < triangles.size(); i++) {
        const Triangle& t = triangles[i];
        cout << "Triangle " << i + 1 << ": ";
        cout << "(" << points[t.a].x << "," << points[t.a].y << ") ";
        cout << "(" << points[t.b].x << "," << points[t.b].y << ") ";
        cout << "(" << points[t.c].x << "," << points[t.c].y << ")" << endl;
    }
}