#ifndef DCEL_QUERIES_H
#define DCEL_QUERIES_H

#include "dcel.h"
#include <vector>

using namespace std;

// This is where you add new query functions
// All query functions should be in this file

class DCELQueries {
private:
    const DCEL* dcel;
    
public:
    DCELQueries(const DCEL* _dcel) : dcel(_dcel) {}
    
    // Vertex queries
    int getVertexDegree(Ident vertexId) const;
    vector<Ident> getAdjacentVertices(Ident vertexId) const;
    vector<Ident> getIncidentEdges(Ident vertexId) const;
    
    // Face queries
    vector<Ident> getFaceVertices(Ident faceId) const;
    vector<Ident> getFaceEdges(Ident faceId) const;
    float getFaceArea(Ident faceId) const;
    bool isFaceBounded(Ident faceId) const;
    int getNumberOfHoles(Ident faceId) const;
    
    // Edge queries
    pair<Ident, Ident> getEdgeEndpoints(Ident edgeId) const;
    float getEdgeLength(Ident edgeId) const;
    pair<Ident, Ident> getAdjacentFaces(Ident edgeId) const;
    
    // Global queries
    int getTotalVertices() const;
    int getTotalEdges() const;
    int getTotalFaces() const;
    bool isConnected() const;
    
    // ADD YOUR NEW QUERY FUNCTIONS HERE
    // Example:
    // bool isVertexOnBoundary(Ident vertexId) const;
    // vector<Ident> getShortestPath(Ident from, Ident to) const;
    // double getAverageEdgeLength() const;
};

#endif