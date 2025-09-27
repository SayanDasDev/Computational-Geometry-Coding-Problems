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
    while (file >> x >> y) {
        points.push_back(Point(x, y));
    }
    
    file.close();
    cout << "Read " << points.size() << " points from " << filename << endl;
    
    return points;
}