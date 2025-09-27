#include "io.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void DCELFileReader::readPointsFile(DCEL& dcel, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open points file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        point p;
        if (iss >> p.id >> p.x >> p.y) {
            dcel.addVertex(p);
        }
    }
    file.close();
}

void DCELFileReader::readSegmentsFile(DCEL& dcel, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open segments file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        istringstream iss(line);
        segment s;
        if (iss >> s.id >> s.start >> s.end) {
            dcel.addSegment(s);
        }
    }
    file.close();
}

void DCELFileReader::readRegionsFile(DCEL& dcel, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open regions file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        region r;
        
        // Read face ID
        istringstream iss(line);
        if (!(iss >> r.id)) continue;
        
        // Read outer boundary
        if (getline(file, line)) {
            if (!line.empty()) {
                istringstream outerIss(line);
                Ident vertexId;
                while (outerIss >> vertexId) {
                    r.outer.push_back(vertexId);
                }
            }
        }
        
        // Read inner boundaries (holes)
        if (getline(file, line)) {
            if (!line.empty()) {
                istringstream innerIss(line);
                Ident vertexId;
                vector<Ident> hole;
                while (innerIss >> vertexId) {
                    hole.push_back(vertexId);
                }
                if (!hole.empty()) {
                    r.inner.push_back(hole);
                }
            }
        }
        
        dcel.addFace(r);
    }
    file.close();
}

void DCELFileReader::buildDCELFromFiles(DCEL& dcel, const string& pointFile, const string& segmentFile, const string& regionFile) {
    cout << "Reading points from " << pointFile << "..." << endl;
    readPointsFile(dcel, pointFile);
    
    cout << "Reading segments from " << segmentFile << "..." << endl;
    readSegmentsFile(dcel, segmentFile);
    
    cout << "Reading regions from " << regionFile << "..." << endl;
    readRegionsFile(dcel, regionFile);
    
    cout << "DCEL construction complete." << endl;
}