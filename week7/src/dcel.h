#ifndef DCEL_H
#define DCEL_H

#include "dcel_types.h"
#include <string>
#include <unordered_map>

using namespace std;

class DCEL {
private:
    unordered_map<Ident, Vertex*> vertices;
    unordered_map<Ident, HalfEdge*> halfEdges;
    unordered_map<Ident, Face*> faces;
    Ident nextHalfEdgeId;
    
    // Private helper methods
    HalfEdge* createBoundary(const vector<Ident>& boundary, Face* face);
    HalfEdge* findHalfEdge(Ident from, Ident to);
    void printBoundary(HalfEdge* start) const;
    
public:
    DCEL();
    ~DCEL();
    
    // Input methods
    void addVertex(const point& p);
    void addSegment(const segment& s);
    void addFace(const region& r);
    void buildFromFile(const string& filename);
    
    // Access methods for queries (const to ensure read-only access)
    const unordered_map<Ident, Vertex*>& getVertices() const { return vertices; }
    const unordered_map<Ident, HalfEdge*>& getHalfEdges() const { return halfEdges; }
    const unordered_map<Ident, Face*>& getFaces() const { return faces; }
    
    Vertex* getVertex(Ident id) const;
    HalfEdge* getHalfEdge(Ident id) const;
    Face* getFace(Ident id) const;
    
    // Basic display methods
    void printVertices() const;
    void printEdges() const;
    void printFaces() const;
};

#endif