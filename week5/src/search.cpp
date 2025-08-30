#include "common.h"

using namespace std;

vector<PointLocation> processQueries(const ConvexPolygon& polygon, const vector<Point>& queryPoints) {
    vector<PointLocation> results;
    results.reserve(queryPoints.size());
    
    for (const Point& q : queryPoints) {
        results.push_back(polygon.queryPoint(q));
    }
    
    return results;
}

void printPolygonChains(const ConvexPolygon& polygon) {
    cout << "\nUpper Chain (Leftmost to Rightmost):\n";
    const auto& upperChain = polygon.getUpperChain();
    for (size_t i = 0; i < upperChain.size(); ++i) {
        cout << "(" << upperChain[i].x << ", " << upperChain[i].y << ")";
        if (i < upperChain.size() - 1) cout << " -> ";
    }
    cout << "\n";
    
    cout << "\nLower Chain (Leftmost to Rightmost):\n";
    const auto& lowerChain = polygon.getLowerChain();
    for (size_t i = 0; i < lowerChain.size(); ++i) {
        cout << "(" << lowerChain[i].x << ", " << lowerChain[i].y << ")";
        if (i < lowerChain.size() - 1) cout << " -> ";
    }
    cout << "\n\n";
}
