#include <iostream>
#include <vector>
#include "point.h"
#include "triangle.h"
#include "triangulator.h"
#include "io.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        cout << "Example: " << argv[0] << " input.in" << endl;
        return 1;
    }
    
    string filename = argv[1];
    
    // Read points from file
    vector<Point> points = FileReader::readPointsFromFile(filename);
    
    if (points.size() < 3) {
        cout << "Error: Need at least 3 points for triangulation" << endl;
        return 1;
    }
    
    // Create triangulator and perform triangulation
    PolygonTriangulator triangulator;
    triangulator.setVertices(points);
    
    vector<Triangle> result = triangulator.triangulate();
    
    if (!result.empty()) {
        triangulator.printTriangles();
        cout << "\nTotal triangles: " << result.size() << endl;
    } else {
        cout << "Triangulation failed!" << endl;
    }
    
    return 0;
}