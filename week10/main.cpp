#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iomanip>
#include <cstdio>

using namespace std;

// Constants for floating point comparisons
const long double EPS = 1e-9;
const long double PI = acos(-1.0);
const long double INF = 1e18;

// Basic structures
struct Point {
    long double x, y;
};

struct Line {
    long double a, b, c; // ax + by + c <= 0
    long double angle;   // Polar angle of the normal/direction
    int id;

    // Calculate angle for sorting
    void computeAngle() {
        // We use the direction vector of the line boundary. 
        // For ax + by + c <= 0, normal is (a,b).
        // The region is to the "left" if we look in direction (-b, a).
        angle = atan2(a, -b); 
    }
};

// Check if a point is "inside" or on boundary of a half-plane
bool isInside(const Line& l, const Point& p) {
    return l.a * p.x + l.b * p.y + l.c <= EPS;
}

// Compute intersection of two lines
// Assumes lines are not parallel
Point intersect(const Line& l1, const Line& l2) {
    long double det = l1.a * l2.b - l2.a * l1.b;
    // If det is 0, lines are parallel (handled elsewhere usually)
    long double x = (l1.b * l2.c - l2.b * l1.c) / det;
    long double y = (l2.a * l1.c - l1.a * l2.c) / det;
    return {x, y};
}

// Comparator for sorting lines by angle
bool compareLines(const Line& l1, const Line& l2) {
    if (abs(l1.angle - l2.angle) > EPS) {
        return l1.angle < l2.angle;
    }
    // If angles are same (parallel), pick the one that is "inner" (more restrictive).
    // Just checking c isn't enough because of signs, we check a point.
    // However, for pure sorting, we can handle duplicates in the main loop.
    return false; // Treat as equal for stable sort, processed later
}

// Check if two lines are parallel
bool areParallel(const Line& l1, const Line& l2) {
    return abs(l1.a * l2.b - l1.b * l2.a) < EPS;
}

// Check if two lines are parallel and point in opposite directions
bool areOpposite(const Line& l1, const Line& l2) {
    if (!areParallel(l1, l2)) return false;
    // Dot product of normals should be negative
    return (l1.a * l2.a + l1.b * l2.b) < 0;
}

void solve() {
    vector<Line> lines;
    long double a, b, c;
    int id_counter = 0;

    // READ UNTIL END OF FILE
    // This loop continues as long as it successfully reads 3 numbers
    while (cin >> a >> b >> c) {
        Line l;
        l.a = a; l.b = b; l.c = c;
        l.computeAngle();
        l.id = id_counter++;
        lines.push_back(l);
    }

    int n = lines.size();
    
    // Safety check for empty files
    if (n == 0) return;

    // --- The rest of the logic remains EXACTLY the same ---

    // Sort by angle
    sort(lines.begin(), lines.end(), compareLines);

    // Remove duplicates (same angle), keep the most restrictive one
    vector<Line> uniqueLines;
    for (int i = 0; i < n; ++i) {
        if (!uniqueLines.empty() && abs(uniqueLines.back().angle - lines[i].angle) < EPS) {
            // Check which is tighter logic (same as before)
            long double d1 = sqrt(uniqueLines.back().a * uniqueLines.back().a + uniqueLines.back().b * uniqueLines.back().b);
            long double d2 = sqrt(lines[i].a * lines[i].a + lines[i].b * lines[i].b);
            long double c1_norm = uniqueLines.back().c / d1;
            long double c2_norm = lines[i].c / d2;
            
            if (c2_norm < c1_norm) {
                uniqueLines.pop_back();
                uniqueLines.push_back(lines[i]);
            }
        } else {
            uniqueLines.push_back(lines[i]);
        }
    }
    lines = uniqueLines;
    n = lines.size(); // Update n after removing duplicates

    // Deque for Half-Plane Intersection
    deque<Line> dq;
    
    for (int i = 0; i < n; ++i) {
        while (dq.size() > 1) {
            Point p = intersect(dq.back(), dq[dq.size() - 2]);
            if (!isInside(lines[i], p)) dq.pop_back();
            else break;
        }
        while (dq.size() > 1) {
            Point p = intersect(dq.front(), dq[1]);
            if (!isInside(lines[i], p)) dq.pop_front();
            else break;
        }
        dq.push_back(lines[i]);
    }

    while (dq.size() > 2) {
        Point p = intersect(dq.back(), dq[dq.size() - 2]);
        if (!isInside(dq.front(), p)) dq.pop_back();
        else break;
    }
    while (dq.size() > 2) {
        Point p = intersect(dq.front(), dq[1]);
        if (!isInside(dq.back(), p)) dq.pop_front();
        else break;
    }

    // --- Classification ---

    if (dq.size() < 2) {
        if (dq.empty()) {
            cout << "INFEASIBLE" << endl;
            return;
        }
        cout << "UNBOUNDED" << endl;
        return;
    }

    bool unbounded = false;
    for (size_t i = 0; i < dq.size(); ++i) {
        size_t next = (i + 1) % dq.size();
        long double ang1 = dq[i].angle;
        long double ang2 = dq[next].angle;
        long double diff = ang2 - ang1;
        if (diff <= 0) diff += 2 * PI;
        
        if (diff > PI + EPS) {
            unbounded = true; 
            break;
        }
    }
    
    if (dq.size() == 2) {
        if (areOpposite(dq[0], dq[1])) {
            Point p;
            if (abs(dq[0].b) > EPS) p = {0, -dq[0].c / dq[0].b};
            else p = {-dq[0].c / dq[0].a, 0};
            
            if (isInside(dq[1], p)) cout << "UNBOUNDED_STRIP" << endl;
            else cout << "INFEASIBLE" << endl;
            return;
        }
    }
    
    vector<Point> vertices;
    for (size_t i = 0; i < dq.size(); ++i) {
        size_t next = (i + 1) % dq.size();
        if (areParallel(dq[i], dq[next])) unbounded = true;
        else vertices.push_back(intersect(dq[i], dq[next]));
    }

    if (unbounded) {
         cout << "UNBOUNDED" << endl;
         return;
    }

    vector<Point> uniqueVerts;
    for(const auto& p : vertices) {
        bool duplicate = false;
        for(const auto& up : uniqueVerts) {
            if (abs(p.x - up.x) < 1e-5 && abs(p.y - up.y) < 1e-5) {
                duplicate = true; 
                break;
            }
        }
        if(!duplicate) uniqueVerts.push_back(p);
    }

    cout << fixed << setprecision(6);
    
    if (uniqueVerts.empty()) {
        cout << "INFEASIBLE" << endl;
    } else if (uniqueVerts.size() == 1) {
        cout << "POINT" << endl;
        cout << uniqueVerts[0].x + 0.0 << " " << uniqueVerts[0].y + 0.0 << endl;
    } else if (uniqueVerts.size() == 2) {
        cout << "LINE_SEGMENT" << endl;
        for(const auto& p : uniqueVerts) cout << p.x + 0.0 << " " << p.y + 0.0 << endl;
    } else {
        cout << "BOUNDED_POLYGON" << endl;
        for(const auto& p : uniqueVerts) cout << p.x + 0.0 << " " << p.y + 0.0 << endl;
    }
}

int main(int argc, char* argv[]) {
    // Check if a filename was provided as a command line argument
    if (argc > 1) {
        // Redirect standard input to read from the file
        if (freopen(argv[1], "r", stdin) == NULL) {
            cerr << "Error: Could not open file " << argv[1] << endl;
            return 1;
        }
    }

    // Fast IO
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    solve();
    
    return 0;
}