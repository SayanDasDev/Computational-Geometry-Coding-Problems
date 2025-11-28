#include "dcel_queries.h"
#include <cmath>
#include <algorithm>
#include <functional>

using namespace std;

// ADD NEW QUERY FUNCTION IMPLEMENTATIONS HERE

int DCELQueries::getVertexDegree(Ident vertexId) const {
    Vertex* v = dcel->getVertex(vertexId);
    if (!v || !v->incidentEdge) return 0;
    
    int degree = 0;
    HalfEdge* start = v->incidentEdge;
    HalfEdge* current = start;
    int safety = 0; // Prevent infinite loops
    
    do {
        if (safety++ > 100) break; // Safety check
        degree++;
        current = current->twin->next;
    } while (current != start && current != nullptr);
    
    return degree;
}

vector<Ident> DCELQueries::getAdjacentVertices(Ident vertexId) const {
    vector<Ident> result;
    Vertex* v = dcel->getVertex(vertexId);
    if (!v || !v->incidentEdge) return result;
    
    HalfEdge* start = v->incidentEdge;
    HalfEdge* current = start;
    int safety = 0; // Prevent infinite loops
    
    do {
        if (safety++ > 100) break; // Safety check
        result.push_back(current->twin->origin->id);
        current = current->twin->next;
    } while (current != start && current != nullptr);
    
    return result;
}

vector<Ident> DCELQueries::getIncidentEdges(Ident vertexId) const {
    vector<Ident> result;
    Vertex* v = dcel->getVertex(vertexId);
    if (!v || !v->incidentEdge) return result;
    
    HalfEdge* start = v->incidentEdge;
    HalfEdge* current = start;
    
    do {
        result.push_back(current->id);
        current = current->twin->next;
    } while (current != start);
    
    return result;
}

vector<Ident> DCELQueries::getFaceVertices(Ident faceId) const {
    vector<Ident> result;
    Face* f = dcel->getFace(faceId);
    if (!f || !f->outerComponent) return result;
    
    HalfEdge* start = f->outerComponent;
    HalfEdge* current = start;
    
    do {
        result.push_back(current->origin->id);
        current = current->next;
    } while (current != start);
    
    return result;
}

vector<Ident> DCELQueries::getFaceEdges(Ident faceId) const {
    vector<Ident> result;
    Face* f = dcel->getFace(faceId);
    if (!f || !f->outerComponent) return result;
    
    HalfEdge* start = f->outerComponent;
    HalfEdge* current = start;
    
    do {
        result.push_back(current->id);
        current = current->next;
    } while (current != start);
    
    return result;
}

float DCELQueries::getFaceArea(Ident faceId) const {
    vector<Ident> vertices = getFaceVertices(faceId);
    if (vertices.size() < 3) return 0.0f;
    
    float area = 0.0f;
    for (size_t i = 0; i < vertices.size(); i++) {
        Vertex* v1 = dcel->getVertex(vertices[i]);
        Vertex* v2 = dcel->getVertex(vertices[(i + 1) % vertices.size()]);
        area += (v1->x * v2->y - v2->x * v1->y);
    }
    return abs(area) / 2.0f;
}

bool DCELQueries::isFaceBounded(Ident faceId) const {
    Face* f = dcel->getFace(faceId);
    return f && f->outerComponent != nullptr;
}

int DCELQueries::getNumberOfHoles(Ident faceId) const {
    Face* f = dcel->getFace(faceId);
    return f ? f->innerComponents.size() : 0;
}

pair<Ident, Ident> DCELQueries::getEdgeEndpoints(Ident edgeId) const {
    HalfEdge* he = dcel->getHalfEdge(edgeId);
    if (!he) return {-1, -1};
    return {he->origin->id, he->twin->origin->id};
}

float DCELQueries::getEdgeLength(Ident edgeId) const {
    HalfEdge* he = dcel->getHalfEdge(edgeId);
    if (!he) return 0.0f;
    
    Vertex* v1 = he->origin;
    Vertex* v2 = he->twin->origin;
    float dx = v2->x - v1->x;
    float dy = v2->y - v1->y;
    return sqrt(dx*dx + dy*dy);
}

pair<Ident, Ident> DCELQueries::getAdjacentFaces(Ident edgeId) const {
    HalfEdge* he = dcel->getHalfEdge(edgeId);
    if (!he) return {-1, -1};
    
    Ident face1 = he->incidentFace ? he->incidentFace->id : -1;
    Ident face2 = he->twin->incidentFace ? he->twin->incidentFace->id : -1;
    return {face1, face2};
}

int DCELQueries::getTotalVertices() const {
    return dcel->getVertices().size();
}

int DCELQueries::getTotalEdges() const {
    return dcel->getHalfEdges().size() / 2; // Each edge has two half-edges
}

int DCELQueries::getTotalFaces() const {
    return dcel->getFaces().size();
}

bool DCELQueries::isConnected() const {
    // Simple connectivity check using DFS
    const auto& vertices = dcel->getVertices();
    if (vertices.empty()) return true;
    if (vertices.size() == 1) return true;
    
    unordered_map<Ident, bool> visited;
    vector<Ident> stack;
    
    // Start DFS from first vertex
    Ident startVertex = vertices.begin()->first;
    stack.push_back(startVertex);
    visited[startVertex] = true;
    size_t visitedCount = 1;
    
    while (!stack.empty()) {
        Ident current = stack.back();
        stack.pop_back();
        
        // Get adjacent vertices more safely
        Vertex* v = dcel->getVertex(current);
        if (!v || !v->incidentEdge) continue;
        
        HalfEdge* start = v->incidentEdge;
        HalfEdge* curr = start;
        int safety = 0; // Prevent infinite loops
        
        do {
            if (safety++ > 100) break; // Safety check
            
            Ident neighbor = curr->twin->origin->id;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                stack.push_back(neighbor);
                visitedCount++;
            }
            curr = curr->twin->next;
        } while (curr != start && curr != nullptr);
    }
    
    return visitedCount == vertices.size();
}

int DCELQueries::getNumberOfEdgesInFace(Ident faceId) const {
    Face* f = dcel->getFace(faceId);
    if (!f || !f->outerComponent) return 0;

    int count = 0;
    HalfEdge* start = f->outerComponent;
    HalfEdge* e = start;

    do {
        count++;
        e = e->next;
    } while (e != start);

    return count;
}

vector<pair<Ident, Ident>> DCELQueries::getDanglingEdges() const {
    vector<pair<Ident, Ident>> result;

    // Compute degree of each vertex
    unordered_map<Ident, int> degree;
    for (auto& [vid, v] : dcel->getVertices()) {
        degree[vid] = 0;
    }

    // Each edge counted once
    for (auto& [eid, he] : dcel->getHalfEdges()) {
        if (eid % 2 == 1) continue; // only one direction
        degree[he->origin->id]++;
        degree[he->twin->origin->id]++;
    }

    // A dangling edge = one endpoint degree = 1
    for (auto& [eid, he] : dcel->getHalfEdges()) {
        if (eid % 2 == 1) continue;
        Ident u = he->origin->id;
        Ident v = he->twin->origin->id;

        if (degree[u] == 1 || degree[v] == 1) {
            result.push_back({u, v});
        }
    }

    return result;
}


vector<pair<Ident, Ident>> DCELQueries::getBridges() const {
    vector<pair<Ident, Ident>> bridges;

    // Build adjacency list
    unordered_map<Ident, vector<Ident>> adj;
    for (auto& [eid, he] : dcel->getHalfEdges()) {
        if (eid % 2 == 1) continue;
        Ident u = he->origin->id;
        Ident v = he->twin->origin->id;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    unordered_map<Ident, int> disc, low;
    unordered_map<Ident, bool> visited;
    int timer = 0;

    function<void(Ident, Ident)> dfs = [&](Ident u, Ident parent) {
        visited[u] = true;
        disc[u] = low[u] = ++timer;

        for (Ident v: adj[u]) {
            if (v == parent) continue;

            if (!visited[v]) {
                dfs(v, u);
                low[u] = min(low[u], low[v]);

                if (low[v] > disc[u]) {
                    bridges.push_back({u, v});
                }
            } else {
                low[u] = min(low[u], disc[v]);
            }
        }
    };

    for (auto& [vid, _] : dcel->getVertices()) {
        if (!visited[vid]) dfs(vid, -1);
    }

    return bridges;
}