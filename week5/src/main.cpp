#include "common.h"
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    string polygonFile, pointsFile;
    

    for (int i = 1; i < argc - 1; ++i) {
        if (strcmp(argv[i], "-p") == 0) {
            polygonFile = argv[i + 1];
        } else if (strcmp(argv[i], "-s") == 0) {
            pointsFile = argv[i + 1];
        }
    }
    
    if (polygonFile.empty() || pointsFile.empty()) {
        cerr << "Usage: " << argv[0] << " -p polygon_file -s points_file\n";
        return 1;
    }

    vector<Point> polygonVertices = readPolygon(polygonFile);
    vector<Point> queryPoints = readPoints(pointsFile);
    
    if (polygonVertices.empty()) {
        cerr << "Error: No polygon vertices found\n";
        return 1;
    }
    
    if (queryPoints.empty()) {
        cerr << "Error: No query points found\n";
        return 1;
    }

    ConvexPolygon polygon;
    polygon.buildFromVertices(polygonVertices);
    

    printPolygonChains(polygon);
    

    vector<PointLocation> results = processQueries(polygon, queryPoints);
    

    printResults(queryPoints, results);
    
    return 0;
}
