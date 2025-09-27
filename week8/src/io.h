#ifndef IO_H
#define IO_H

#include <vector>
#include <string>
#include "point.h"

using namespace std;

class FileReader {
public:
    static vector<Point> readPointsFromFile(const string& filename);
};

#endif