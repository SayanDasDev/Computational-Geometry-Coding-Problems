#include "dcel.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

DCEL::DCEL() : nextHalfEdgeId(0) {}

DCEL::~DCEL() {
    for (auto& pair : vertices) delete pair.second;
    for (auto& pair : halfEdges) delete pair.second;
    for (auto& pair : faces) delete pair.second;
}

void DCEL::addVertex(const point& p) {
    vertices[p.id] = new Vertex(p.id, p.x, p.y);
}

void DCEL::addSegment(const segment& s) {
    // Create two half-edges for each segment
    HalfEdge* he1 = new HalfEdge(nextHalfEdgeId++);
    HalfEdge* he2 = new HalfEdge(nextHalfEdgeId++);
    
    // Set origins
    he1->origin = vertices[s.start];
    he2->origin = vertices[s.end];
    
    // Set twins
    he1->twin = he2;
    he2->twin = he1;
    
    // Set incident edges for vertices if not set
    if (vertices[s.start]->incidentEdge == nullptr) {
        vertices[s.start]->incidentEdge = he1;
    }
    if (vertices[s.end]->incidentEdge == nullptr) {
        vertices[s.end]->incidentEdge = he2;
    }
    
    halfEdges[he1->id] = he1;
    halfEdges[he2->id] = he2;
}

void DCEL::addFace(const region& r) {
    Face* face = new Face(r.id);
    faces[r.id] = face;
    
    // Process outer boundary
    if (!r.outer.empty()) {
        face->outerComponent = createBoundary(r.outer, face);
    }
    
    // Process inner boundaries (holes)
    for (const auto& hole : r.inner) {
        if (!hole.empty()) {
            HalfEdge* holeEdge = createBoundary(hole, face);
            face->innerComponents.push_back(holeEdge);
        }
    }
}

void DCEL::buildFromFile(const string& filename) {
    ifstream file(filename);
    string line;
    string section;
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        if (line == "[POINTS]") {
            section = "points";
            continue;
        } else if (line == "[SEGMENTS]") {
            section = "segments";
            continue;
        } else if (line == "[REGIONS]") {
            section = "regions";
            continue;
        }
        
        istringstream iss(line);
        
        if (section == "points") {
            point p;
            iss >> p.id >> p.x >> p.y;
            addVertex(p);
        } else if (section == "segments") {
            segment s;
            iss >> s.id >> s.start >> s.end;
            addSegment(s);
        } else if (section == "regions") {
            region r;
            iss >> r.id;
            
            // Read outer boundary
            string token;
            iss >> token; // should be "OUTER"
            int outerSize;
            iss >> outerSize;
            r.outer.resize(outerSize);
            for (int i = 0; i < outerSize; i++) {
                iss >> r.outer[i];
            }
            
            // Read holes
            while (iss >> token && token == "HOLE") {
                int holeSize;
                iss >> holeSize;
                vector<Ident> hole(holeSize);
                for (int i = 0; i < holeSize; i++) {
                    iss >> hole[i];
                }
                r.inner.push_back(hole);
            }
            
            addFace(r);
        }
    }
}

Vertex* DCEL::getVertex(Ident id) const {
    auto it = vertices.find(id);
    return (it != vertices.end()) ? it->second : nullptr;
}

HalfEdge* DCEL::getHalfEdge(Ident id) const {
    auto it = halfEdges.find(id);
    return (it != halfEdges.end()) ? it->second : nullptr;
}

Face* DCEL::getFace(Ident id) const {
    auto it = faces.find(id);
    return (it != faces.end()) ? it->second : nullptr;
}

HalfEdge* DCEL::createBoundary(const vector<Ident>& boundary, Face* face) {
    if (boundary.size() < 2) return nullptr;
    
    vector<HalfEdge*> boundaryEdges;
    
    // Find the half-edges for this boundary
    for (size_t i = 0; i < boundary.size(); i++) {
        Ident from = boundary[i];
        Ident to = boundary[(i + 1) % boundary.size()];
        
        HalfEdge* edge = findHalfEdge(from, to);
        if (edge) {
            boundaryEdges.push_back(edge);
            edge->incidentFace = face;
        }
    }
    
    // Link the edges
    for (size_t i = 0; i < boundaryEdges.size(); i++) {
        boundaryEdges[i]->next = boundaryEdges[(i + 1) % boundaryEdges.size()];
        boundaryEdges[i]->prev = boundaryEdges[(i + boundaryEdges.size() - 1) % boundaryEdges.size()];
    }
    
    return boundaryEdges.empty() ? nullptr : boundaryEdges[0];
}

HalfEdge* DCEL::findHalfEdge(Ident from, Ident to) {
    for (const auto& pair : halfEdges) {
        HalfEdge* he = pair.second;
        if (he->origin->id == from && he->twin->origin->id == to) {
            return he;
        }
    }
    return nullptr;
}

void DCEL::printVertices() const {
    cout << "Vertices:\n";
    for (const auto& pair : vertices) {
        const Vertex* v = pair.second;
        cout << "  ID: " << v->id << ", Position: (" << v->x << ", " << v->y << ")\n";
    }
}

void DCEL::printEdges() const {
    cout << "Half-Edges:\n";
    unordered_map<int, bool> printed;
    for (const auto& pair : halfEdges) {
        HalfEdge* he = pair.second;
        if (printed[he->id]) continue;
        
        cout << "  Edge: " << he->origin->id << " -> " << he->twin->origin->id;
        cout << " (HE IDs: " << he->id << ", " << he->twin->id << ")\n";
        
        printed[he->id] = true;
        printed[he->twin->id] = true;
    }
}

void DCEL::printFaces() const {
    cout << "Faces:\n";
    for (const auto& pair : faces) {
        const Face* f = pair.second;
        cout << "  Face ID: " << f->id << "\n";
        
        if (f->outerComponent) {
            cout << "    Outer boundary: ";
            printBoundary(f->outerComponent);
        } else {
            cout << "    Unbounded face\n";
        }
        
        for (size_t i = 0; i < f->innerComponents.size(); i++) {
            cout << "    Hole " << i + 1 << ": ";
            printBoundary(f->innerComponents[i]);
        }
    }
}

void DCEL::printBoundary(HalfEdge* start) const {
    if (!start) {
        cout << "None\n";
        return;
    }
    
    HalfEdge* current = start;
    do {
        cout << current->origin->id;
        current = current->next;
        if (current != start) cout << " -> ";
    } while (current != start);
    cout << "\n";
}