#ifndef IO_H
#define IO_H

#include "dcel.h"
#include <string>

using namespace std;

class DCELFileReader {
public:
    static void readPointsFile(DCEL& dcel, const string& filename);
    static void readSegmentsFile(DCEL& dcel, const string& filename);
    static void readRegionsFile(DCEL& dcel, const string& filename);
    static void buildDCELFromFiles(DCEL& dcel, const string& pointFile, const string& segmentFile, const string& regionFile);
};

#endif