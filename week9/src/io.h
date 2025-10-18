#ifndef IO_H
#define IO_H

#include <vector>
#include <string>
#include "point.h"

using namespace std;

struct Triangle {
    int a, b, c;
    Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};

class FileReader {
public:
    static vector<Point> readPointsFromFile(const string& filename);
};

class OutputWriter {
public:
    static void writeTriangles(const vector<Triangle>& triangles, const vector<Point>& points);
};

#endif