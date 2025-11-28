#include "io.h"
#include "triangle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// FileInputSource implementation
FileInputSource::FileInputSource(const string& filename, bool verbose) 
    : filename(filename), verbose(verbose) {}

vector<Point> FileInputSource::readPoints() {
    vector<Point> points;
    ifstream file(filename);
    
    if (!file.is_open()) {
        if (verbose) {
            cout << "Error: Could not open file " << filename << endl;
        }
        return points;
    }
    
    double x, y;
    while (file >> x >> y) {
        points.push_back(Point(x, y));
    }
    
    file.close();
    
    if (verbose) {
        cout << "Read " << points.size() << " points from " << filename << endl;
    }
    
    return points;
}

bool FileInputSource::isValid() const {
    ifstream file(filename);
    return file.good();
}

string FileInputSource::getSourceInfo() const {
    return "File: " + filename;
}

// ScriptInputSource implementation
ScriptInputSource::ScriptInputSource(const string& scriptPath, bool verbose) 
    : scriptPath(scriptPath), verbose(verbose) {
    command = "python \"" + scriptPath + "\"";
}

vector<Point> ScriptInputSource::readPoints() {
    vector<Point> points;
    
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        if (verbose) {
            cout << "Error: Could not execute script " << scriptPath << endl;
        }
        return points;
    }
    
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    
    // Parse the output
    istringstream iss(result);
    double x, y;
    while (iss >> x >> y) {
        points.push_back(Point(x, y));
    }
    
    if (verbose) {
        cout << "Generated " << points.size() << " points from " << scriptPath << endl;
    }
    
    return points;
}

bool ScriptInputSource::isValid() const {
    ifstream file(scriptPath);
    return file.good();
}

string ScriptInputSource::getSourceInfo() const {
    return "Script: " + scriptPath;
}

// ConsoleOutput implementation
ConsoleOutput::ConsoleOutput(bool verbose) : verbose(verbose) {}

void ConsoleOutput::writePoints(const vector<Point>& points) {
    if (verbose) {
        cout << "\nInput points = [";
        for (size_t i = 0; i < points.size(); i++) {
            cout << "(" << points[i].x << ", " << points[i].y << ")" << ",";
        }
        cout << "]" << endl;
    }
}

void ConsoleOutput::writeTriangles(const vector<Triangle>& triangles, const vector<Point>& vertices) {
    if (verbose) {
        cout << "Triangulation result:" << endl;
        for (size_t i = 0; i < triangles.size(); i++) {
            const Triangle& t = triangles[i];
            cout << "Triangle " << i + 1 << ": (" 
                 << vertices[t.a].x << "," << vertices[t.a].y << ") ("
                 << vertices[t.b].x << "," << vertices[t.b].y << ") ("
                 << vertices[t.c].x << "," << vertices[t.c].y << ")" << endl;
        }
        // cout << "\nTotal triangles: " << triangles.size() << endl;
    }
}

bool ConsoleOutput::isValid() const {
    return true; // Console is always available
}

// FileOutput implementation
FileOutput::FileOutput(const string& filename, bool verbose) 
    : filename(filename), verbose(verbose) {}

void FileOutput::writePoints(const vector<Point>& points) {
    ofstream file(filename + "_points.txt");
    if (!file.is_open()) {
        if (verbose) {
            cout << "Error: Could not create points output file" << endl;
        }
        return;
    }
    
    for (const auto& point : points) {
        file << point.x << " " << point.y << endl;
    }
    
    file.close();
    if (verbose) {
        cout << "Points written to " << filename + "_points.txt" << endl;
    }
}

void FileOutput::writeTriangles(const vector<Triangle>& triangles, const vector<Point>& vertices) {
    ofstream file(filename + "_triangles.txt");
    if (!file.is_open()) {
        if (verbose) {
            cout << "Error: Could not create triangles output file" << endl;
        }
        return;
    }
    
    for (size_t i = 0; i < triangles.size(); i++) {
        const Triangle& t = triangles[i];
        file << "Triangle " << i + 1 << ": " 
             << vertices[t.a].x << " " << vertices[t.a].y << " "
             << vertices[t.b].x << " " << vertices[t.b].y << " "
             << vertices[t.c].x << " " << vertices[t.c].y << endl;
    }
    
    file.close();
    if (verbose) {
        cout << "Triangles written to " << filename + "_triangles.txt" << endl;
    }
}

bool FileOutput::isValid() const {
    ofstream file(filename + "_test.tmp");
    bool valid = file.good();
    file.close();
    remove((filename + "_test.tmp").c_str());
    return valid;
}

// IOFactory implementation
unique_ptr<InputSource> IOFactory::createFileInput(const string& filename, bool verbose) {
    return make_unique<FileInputSource>(filename, verbose);
}

unique_ptr<InputSource> IOFactory::createScriptInput(const string& scriptPath, bool verbose) {
    return make_unique<ScriptInputSource>(scriptPath, verbose);
}

unique_ptr<OutputDestination> IOFactory::createConsoleOutput(bool verbose) {
    return make_unique<ConsoleOutput>(verbose);
}

unique_ptr<OutputDestination> IOFactory::createFileOutput(const string& filename, bool verbose) {
    return make_unique<FileOutput>(filename, verbose);
}

// Backward compatibility implementation
vector<Point> FileReader::readPointsFromFile(const string& filename) {
    auto input = IOFactory::createFileInput(filename, true);
    return input->readPoints();
}