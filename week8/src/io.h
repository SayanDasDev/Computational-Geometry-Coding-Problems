#ifndef IO_H
#define IO_H

#include <vector>
#include <string>
#include <memory>
#include "point.h"

using namespace std;

// Forward declarations
class Triangle;

// Abstract base class for input sources
class InputSource {
public:
    virtual ~InputSource() = default;
    virtual vector<Point> readPoints() = 0;
    virtual bool isValid() const = 0;
    virtual string getSourceInfo() const = 0;
};

// Abstract base class for output destinations
class OutputDestination {
public:
    virtual ~OutputDestination() = default;
    virtual void writePoints(const vector<Point>& points) = 0;
    virtual void writeTriangles(const vector<Triangle>& triangles, const vector<Point>& vertices) = 0;
    virtual bool isValid() const = 0;
};

// File-based input implementation
class FileInputSource : public InputSource {
private:
    string filename;
    bool verbose;

public:
    explicit FileInputSource(const string& filename, bool verbose = true);
    vector<Point> readPoints() override;
    bool isValid() const override;
    string getSourceInfo() const override;
};

// Command/Script-based input implementation
class ScriptInputSource : public InputSource {
private:
    string scriptPath;
    string command;
    bool verbose;

public:
    explicit ScriptInputSource(const string& scriptPath, bool verbose = true);
    vector<Point> readPoints() override;
    bool isValid() const override;
    string getSourceInfo() const override;
};

// Console output implementation
class ConsoleOutput : public OutputDestination {
private:
    bool verbose;

public:
    explicit ConsoleOutput(bool verbose = true);
    void writePoints(const vector<Point>& points) override;
    void writeTriangles(const vector<Triangle>& triangles, const vector<Point>& vertices) override;
    bool isValid() const override;
};

// File output implementation
class FileOutput : public OutputDestination {
private:
    string filename;
    bool verbose;

public:
    explicit FileOutput(const string& filename, bool verbose = true);
    void writePoints(const vector<Point>& points) override;
    void writeTriangles(const vector<Triangle>& triangles, const vector<Point>& vertices) override;
    bool isValid() const override;
};

// Factory class for creating input/output objects
class IOFactory {
public:
    static unique_ptr<InputSource> createFileInput(const string& filename, bool verbose = true);
    static unique_ptr<InputSource> createScriptInput(const string& scriptPath, bool verbose = true);
    static unique_ptr<OutputDestination> createConsoleOutput(bool verbose = true);
    static unique_ptr<OutputDestination> createFileOutput(const string& filename, bool verbose = true);
};

// Utility class for backward compatibility
class FileReader {
public:
    static vector<Point> readPointsFromFile(const string& filename);
};

#endif