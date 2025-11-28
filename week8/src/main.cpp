#include <iostream>
#include <vector>
#include "point.h"
#include "triangle.h"
#include "triangulator.h"
#include "io.h"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        cout << "       " << argv[0] << " --rand [python_script_path]" << endl;
        cout << "Example: " << argv[0] << " input.in" << endl;
        cout << "         " << argv[0] << " --rand" << endl;
        cout << "         " << argv[0] << " --rand ./custom-gen.py" << endl;
        return 1;
    }
    
    string arg1 = argv[1];
    
    
    unique_ptr<InputSource> inputSource;
    unique_ptr<OutputDestination> outputDest = IOFactory::createConsoleOutput(true);
    
    if (arg1 == "--rand") {
        string scriptPath = "../test/gen-rand.py";
        
        if (argc >= 3) {
            scriptPath = argv[2];
        }
        
        inputSource = IOFactory::createScriptInput(scriptPath, true);
    } else {
        inputSource = IOFactory::createFileInput(arg1, true);
    }
    
    if (!inputSource->isValid()) {
        cout << "Error: Invalid input source - " << inputSource->getSourceInfo() << endl;
        return 1;
    }
    
    vector<Point> points = inputSource->readPoints();
    
    if (points.size() < 3) {
        cout << "Error: Need at least 3 points for triangulation" << endl;
        return 1;
    }
    
    outputDest->writePoints(points);
    
    PolygonTriangulator triangulator;
    triangulator.setVertices(points);
    
    vector<Triangle> result = triangulator.triangulate();
    
    if (!result.empty()) {
        outputDest->writeTriangles(result, points);
    } else {
        cout << "Triangulation failed!" << endl;
    }
    
    return 0;
}