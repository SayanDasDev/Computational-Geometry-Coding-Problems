#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

const double EPS = 1e-2;

struct Point {
    double x, y;
    
    Point() : x(0.0), y(0.0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }
};

int orientation(const Point& p, const Point& q, const Point& r) {
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (abs(val) < EPS) return 0;
    return (val > 0) ? 1 : 2;
}

double distSquared(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

vector<Point> convexHull(vector<Point>& points) {
    int n = points.size();
    
    if (n < 3) return points;
    
    int l = 0;
    for (int i = 1; i < n; i++) {
        if (points[i].x < points[l].x)
            l = i;
        else if (points[i].x == points[l].x && points[i].y < points[l].y)
            l = i;
    }
    
    vector<Point> hull;
    int p = l, q;
    
    do {
        hull.push_back(points[p]);
        
        q = (p + 1) % n;
        
        for (int i = 0; i < n; i++) {
            int orient = orientation(points[p], points[i], points[q]);
            
            if (orient == 2) {
                q = i;
            }
            else if (orient == 0 && distSquared(points[p], points[i]) > distSquared(points[p], points[q])) {
                q = i;
            }
        }
        
        p = q;
        
    } while (p != l);
    
    return hull;
}

vector<Point> readPointsFromFile(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return points;
    }
    
    double x, y;
    while (file >> x >> y) {
        points.emplace_back(x, y);
    }
    
    file.close();
    return points;
}

void printPoints(const vector<Point>& points) {
    for (const Point& p : points) {
        cout << p.x << " " << p.y << endl;
    }
}

int main(int argc, char* argv[]) {
    string filename;
    if (argc > 1) {
        filename = argv[1];
    } else {
        cout << "Enter filename: ";
        cin >> filename;
    }
    
    vector<Point> points = readPointsFromFile(filename);
    
    if (points.empty()) {
        cerr << "No points found in file or file could not be read." << endl;
        return 1;
    }
    
    cout << "Input points (" << points.size() << " points):" << endl;
    printPoints(points);
    cout << endl;
    
    vector<Point> hull = convexHull(points);
    
    cout << "Convex Hull (" << hull.size() << " points):" << endl;
    printPoints(hull);
    
    return 0;
}