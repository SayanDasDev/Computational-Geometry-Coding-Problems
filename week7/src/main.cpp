#include "dcel.h"
#include "dcel_queries.h"
#include "io.h"
#include <iostream>
#include <cstring>

using namespace std;

void printUsage() {
    cout << "Usage: ./dcel -p <point_file> -s <segment_file> -r <region_file>\n";
    cout << "Example: ./dcel -p point.in -s segment.in -r region.in\n";
}

int main(int argc, char* argv[]) {
    string pointFile, segmentFile, regionFile;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pointFile = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            segmentFile = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            regionFile = argv[++i];
        } else {
            printUsage();
            return 1;
        }
    }
    
    if (pointFile.empty() || segmentFile.empty() || regionFile.empty()) {
        printUsage();
        return 1;
    }
    
    DCEL dcel;
    
    cout << "Building DCEL from files:\n";
    cout << "  Points: " << pointFile << "\n";
    cout << "  Segments: " << segmentFile << "\n";
    cout << "  Regions: " << regionFile << "\n\n";
    
    // Build DCEL from files using DCELFileReader
    DCELFileReader::buildDCELFromFiles(dcel, pointFile, segmentFile, regionFile);
    cout << "\n";
    
    // Print basic structure
    dcel.printVertices();
    cout << "\n";
    dcel.printEdges();
    cout << "\n";
    dcel.printFaces();
    cout << "\n";
    
    // Create query object
    DCELQueries queries(&dcel);
    
    // Test queries
    cout << "Query Results:\n";
    cout << "Total vertices: " << queries.getTotalVertices() << "\n";
    cout << "Total edges: " << queries.getTotalEdges() << "\n";
    cout << "Total faces: " << queries.getTotalFaces() << "\n";
    cout << "Is connected: " << (queries.isConnected() ? "Yes" : "No") << "\n";
    
    // Query specific vertices if they exist
    if (queries.getTotalVertices() > 0) {
        Ident firstVertexId = dcel.getVertices().begin()->first;
        cout << "Vertex " << firstVertexId << " degree: " << queries.getVertexDegree(firstVertexId) << "\n";
        
        auto adjacentVertices = queries.getAdjacentVertices(firstVertexId);
        cout << "Vertex " << firstVertexId << " adjacent vertices: ";
        for (Ident v : adjacentVertices) {
            cout << v << " ";
        }
        cout << "\n";
    }
    
    // Query specific faces if they exist
    if (queries.getTotalFaces() > 0) {
        for (const auto& facePair : dcel.getFaces()) {
            Ident faceId = facePair.first;
            cout << "Face " << faceId << " is bounded: " << (queries.isFaceBounded(faceId) ? "Yes" : "No") << "\n";
            if (queries.isFaceBounded(faceId)) {
                cout << "Face " << faceId << " area: " << queries.getFaceArea(faceId) << "\n";
                auto faceVertices = queries.getFaceVertices(faceId);
                cout << "Face " << faceId << " vertices: ";
                for (Ident v : faceVertices) {
                    cout << v << " ";
                }
                cout << "\n";
            }
        }
    }
    
    return 0;
}