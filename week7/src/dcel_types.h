#ifndef DCEL_TYPES_H
#define DCEL_TYPES_H

#include <vector>
#include <unordered_map>

using namespace std;

typedef int Ident;

// Input structures
struct point {
    Ident id;
    float x, y;
};

struct segment {
    Ident id;
    Ident start;
    Ident end;
};

struct region {
    Ident id;
    vector<Ident> outer;  // anti-clockwise
    vector<vector<Ident>> inner;  // clockwise holes
};

// Forward declarations
struct Vertex;
struct HalfEdge;
struct Face;

// DCEL structures
struct Vertex {
    Ident id;
    float x, y;
    HalfEdge* incidentEdge;  // one of the half-edges originating from this vertex
    
    Vertex(Ident _id, float _x, float _y) : id(_id), x(_x), y(_y), incidentEdge(nullptr) {}
};

struct HalfEdge {
    Ident id;
    Vertex* origin;          // vertex this edge starts from
    HalfEdge* twin;          // opposite half-edge
    HalfEdge* next;          // next edge in face boundary
    HalfEdge* prev;          // previous edge in face boundary
    Face* incidentFace;      // face to the left of this edge
    
    HalfEdge(Ident _id) : id(_id), origin(nullptr), twin(nullptr), next(nullptr), prev(nullptr), incidentFace(nullptr) {}
};

struct Face {
    Ident id;
    HalfEdge* outerComponent;  // outer boundary (nullptr for unbounded face)
    vector<HalfEdge*> innerComponents;  // holes
    
    Face(Ident _id) : id(_id), outerComponent(nullptr) {}
};

#endif