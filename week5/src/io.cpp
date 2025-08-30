#include "common.h"

using namespace std;

vector<Point> readPolygon(const string& filename) {
    vector<Point> vertices;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open polygon file " << filename << endl;
        return vertices;
    }
    
    double x, y;
    while (file >> x >> y) {
        vertices.emplace_back(x, y);
    }
    
    file.close();
    return vertices;
}

vector<Point> readPoints(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Cannot open points file " << filename << endl;
        return points;
    }
    
    double x, y;
    while (file >> x >> y) {
        points.emplace_back(x, y);
    }
    
    file.close();
    return points;
}

void printResults(const vector<Point>& points, const vector<PointLocation>& results) {
    cout << "Query Results:\n";
    cout << "Point\t\tLocation\n";
    cout << "-----\t\t--------\n";
    
    for (size_t i = 0; i < points.size(); ++i) {
        cout << "(" << points[i].x << ", " << points[i].y << ")\t";
        
        switch (results[i]) {
            case INSIDE:
                cout << "INSIDE\n";
                break;
            case OUTSIDE:
                cout << "OUTSIDE\n";
                break;
            case ON_BOUNDARY:
                cout << "ON_BOUNDARY\n";
                break;
        }
    }
}
