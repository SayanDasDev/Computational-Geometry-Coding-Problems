#include "common.h"

using namespace std;

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

bool onSegment(const Point& a, const Point& b, const Point& c) {
    const double EPS = 1e-9;
    
    Point ab = b - a;
    Point ac = c - a;
    if (abs(cross(ab, ac)) > EPS) return false;
    
    double minX = min(a.x, b.x) - EPS;
    double maxX = max(a.x, b.x) + EPS;
    double minY = min(a.y, b.y) - EPS;
    double maxY = max(a.y, b.y) + EPS;
    
    return c.x >= minX && c.x <= maxX && c.y >= minY && c.y <= maxY;
}

bool compareByX(const Point& a, const Point& b) {
    if (abs(a.x - b.x) < 1e-9) {
        return a.y < b.y;
    }
    return a.x < b.x;
}

void ConvexPolygon::buildFromVertices(const vector<Point>& vertices) {
    if (vertices.size() < 3) return;
    
    auto minMaxX = minmax_element(vertices.begin(), vertices.end(), compareByX);
    leftmost = *minMaxX.first;
    rightmost = *minMaxX.second;
    
    upperChain.clear();
    lowerChain.clear();
    
    
    upperChain.push_back(leftmost);
    lowerChain.push_back(leftmost);
    
    vector<Point> upperPoints, lowerPoints;
    
    for (const Point& p : vertices) {
        if (p == leftmost || p == rightmost) continue;
        
        Point leftToRight = rightmost - leftmost;
        Point leftToP = p - leftmost;
        double crossProduct = cross(leftToRight, leftToP);
        
        const double EPS = 1e-9;
        if (crossProduct > EPS) {
            upperPoints.push_back(p);
        } else if (crossProduct < -EPS) {
            lowerPoints.push_back(p);
        } else {
            upperPoints.push_back(p);
            lowerPoints.push_back(p);
        }
    }
    
    sort(upperPoints.begin(), upperPoints.end(), compareByX);
    sort(lowerPoints.begin(), lowerPoints.end(), compareByX);
    
    for (const Point& p : upperPoints) {
        upperChain.push_back(p);
    }
    for (const Point& p : lowerPoints) {
        lowerChain.push_back(p);
    }
    
    upperChain.push_back(rightmost);
    lowerChain.push_back(rightmost);
}

PointLocation ConvexPolygon::queryPoint(const Point& q) const {
    const double EPS = 1e-9;
    
    if (q.x < leftmost.x - EPS || q.x > rightmost.x + EPS) {
        return OUTSIDE;
    }
    
    if (q == leftmost || q == rightmost) {
        return ON_BOUNDARY;
    }
    
    int left = 0, right = upperChain.size() - 1;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (upperChain[mid].x <= q.x) {
            left = mid;
        } else {
            right = mid;
        }
    }
    
    if (onSegment(upperChain[left], upperChain[right], q)) {
        return ON_BOUNDARY;
    }
    
    Point edge = upperChain[right] - upperChain[left];
    Point toQuery = q - upperChain[left];
    double upperCross = cross(edge, toQuery);
    
    if (upperCross > EPS) {
        return OUTSIDE;
    }
    
    left = 0, right = lowerChain.size() - 1;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (lowerChain[mid].x <= q.x) {
            left = mid;
        } else {
            right = mid;
        }
    }
    
    if (onSegment(lowerChain[left], lowerChain[right], q)) {
        return ON_BOUNDARY;
    }
    
    edge = lowerChain[right] - lowerChain[left];
    toQuery = q - lowerChain[left];
    double lowerCross = cross(edge, toQuery);
    
    if (lowerCross < -EPS) {
        return OUTSIDE;
    }
    
    return INSIDE;
}