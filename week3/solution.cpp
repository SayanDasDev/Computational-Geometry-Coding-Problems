#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

const float EPS = 1e-9;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

class PointInPolygon {
private:
    vector<Point> polygon;
    
    bool isOnEdge(const Point& p, const Point& a, const Point& b) const {
        double cross = (p.y - a.y) * (b.x - a.x) - (p.x - a.x) * (b.y - a.y);
        if (fabs(cross) > EPS) return false;
        
        double minX = min(a.x, b.x);
        double maxX = max(a.x, b.x);
        double minY = min(a.y, b.y);
        double maxY = max(a.y, b.y);
        
        return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
    }
    
    bool isInside(const Point& point) const {
        int intersections = 0;
        int n = polygon.size();
        
        for (int i = 0; i < n; i++) {
            Point a = polygon[i];
            Point b = polygon[(i + 1) % n];
            
            if (isOnEdge(point, a, b)) {
                return false;
            }
            
            if (fabs(a.y - b.y) < EPS) continue;
            
            double minY = min(a.y, b.y);
            double maxY = max(a.y, b.y);
            
            if (point.y < minY || point.y >= maxY) continue;
            
            double intersectionX = a.x + (point.y - a.y) * (b.x - a.x) / (b.y - a.y);
            
            if (intersectionX > point.x) {
                intersections++;
            }
        }
        
        return (intersections % 2) == 1;
    }
    
public:
    void setPolygon(const vector<Point>& poly) {
        polygon = poly;
    }
    
    bool checkPoint(const Point& point) const {
        return isInside(point);
    }
};

int main(int argc, char* argv[]) {
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        cout << "Enter filename: ";
        cin >> filename;
    }
    
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open " << filename << endl;
        return 1;
    }
    
    vector<Point> polygonPoints;
    vector<Point> queryPoints;
    string line;
    bool readingPolygon = true;
    
    while (getline(file, line)) {
        if (line.empty()) {
            readingPolygon = false;
            continue;
        }
        
        istringstream iss(line);
        double x, y;
        if (iss >> x >> y) {
            if (readingPolygon) {
                polygonPoints.push_back(Point(x, y));
            } else {
                queryPoints.push_back(Point(x, y));
            }
        }
    }
    file.close();
    
    if (polygonPoints.size() < 3) {
        cerr << "Error: Polygon must have at least 3 points" << endl;
        return 1;
    }
    
    if (queryPoints.empty()) {
        cerr << "Error: No query points found" << endl;
        return 1;
    }
    
    PointInPolygon checker;
    checker.setPolygon(polygonPoints);
    
    cout << "Polygon points: " << polygonPoints.size() << endl;
    cout << "Query points: " << queryPoints.size() << endl;
    cout << "\nResults:" << endl;
    
    bool allInside = true;
    for (size_t i = 0; i < queryPoints.size(); i++) {
        bool inside = checker.checkPoint(queryPoints[i]);
        cout << "Point " << (i+1) << " (" << queryPoints[i].x << ", " 
                  << queryPoints[i].y << "): " << (inside ? "INSIDE" : "OUTSIDE/ON BOUNDARY") << endl;
        if (!inside) {
            allInside = false;
        }
    }
    
    cout << "\nOverall result: " << (allInside ? "ALL POINTS ARE IN INTERIOR" : "NOT ALL POINTS ARE IN INTERIOR") << endl;
    
    return 0;
}