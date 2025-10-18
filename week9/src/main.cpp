#include <iostream>
#include <vector>
#include "point.h"
#include "y_monotone.h"
#include "io.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        cout << "Example: " << argv[0] << " input.txt" << endl;
        return 1;
    }
    
    string filename = argv[1];
    
    vector<Point> points = FileReader::readPointsFromFile(filename);
    
    if (points.size() < 3) {
        cout << "Error: Need at least 3 points for triangulation" << endl;
        return 1;
    }
    
    cout << "Input polygon vertices:" << endl;
    for (size_t i = 0; i < points.size(); i++) {
        cout << "Vertex " << i << ": " << points[i] << endl;
    }
    cout << endl;
    
    YMonotoneTriangulator triangulator(points);
    vector<Triangle> result = triangulator.triangulate();
    
    if (!result.empty()) {
        triangulator.printTriangulation();
        cout << "\nTotal triangles: " << result.size() << endl;
        cout << "Expected triangles for " << points.size() << " vertices: " << (points.size() - 2) << endl;
    } else {
        cout << "Triangulation faileds!" << endl;
    }
    
    return 0;
}