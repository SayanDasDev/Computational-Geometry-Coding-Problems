#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
using namespace std;

const float EPS = 1e-3;

struct pts
{
    float x, y;
    int originalIndex;

    pts(float x = 0, float y = 0, int idx = -1) : x(x), y(y), originalIndex(idx) {}

    bool operator==(const pts &other) const
    {
        return fabs(x - other.x) < EPS && fabs(y - other.y) < EPS;
    }

    void print() const
    {
        cout << "(" << x << ", " << y << ")";
    }
};

struct Line
{
    float m, c;
    bool isVertical;
    float verticalX;
    vector<pts> points;

    Line() : m(0), c(0), isVertical(false), verticalX(0) {}

    void print() const
    {
        if (isVertical)
        {
            cout << "Vertical line x = " << verticalX << " contains points: ";
        }
        else
        {
            cout << "Line y = " << m << "x + " << c << " contains points: ";
        }
        for (size_t i = 0; i < points.size(); i++)
        {
            points[i].print();
            if (i < points.size() - 1)
                cout << ", ";
        }
        cout << endl;
    }
};

vector<pts> readPoints(const string &filename)
{
    vector<pts> points;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << endl;
        return points;
    }

    float x, y;
    char comma;
    int index = 0;
    while (file >> x >> comma >> y)
    {
        points.push_back(pts(x, y, index));
        index++;
    }

    file.close();
    return points;
}

struct CoincidentGroup
{
    pts representative;
    vector<int> originalIndices;

    void print() const
    {
        representative.print();
        cout << " appears " << originalIndices.size() << " times at original positions: ";
        for (size_t i = 0; i < originalIndices.size(); i++)
        {
            cout << originalIndices[i];
            if (i < originalIndices.size() - 1)
                cout << ", ";
        }
        cout << endl;
    }
};

vector<CoincidentGroup> findCoincidentGroups(const vector<pts> &points)
{
    vector<CoincidentGroup> coincidentGroups;

    for (size_t i = 0; i < points.size(); i++)
    {
        bool alreadyGrouped = false;
        for (auto &group : coincidentGroups)
        {
            if (points[i] == group.representative)
            {
                group.originalIndices.push_back(points[i].originalIndex);
                alreadyGrouped = true;
                break;
            }
        }

        if (!alreadyGrouped)
        {
            CoincidentGroup newGroup;
            newGroup.representative = points[i];
            newGroup.originalIndices.push_back(points[i].originalIndex);

            for (size_t j = i + 1; j < points.size(); j++)
            {
                if (points[i] == points[j])
                {
                    newGroup.originalIndices.push_back(points[j].originalIndex);
                }
            }

            coincidentGroups.push_back(newGroup);
        }
    }

    return coincidentGroups;
}

vector<pts> removeDuplicates(const vector<pts> &points)
{
    vector<pts> unique_points;

    for (const auto &point : points)
    {
        bool isDuplicate = false;
        for (const auto &unique_point : unique_points)
        {
            if (point == unique_point)
            {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate)
        {
            unique_points.push_back(point);
        }
    }

    return unique_points;
}

bool arePointsOnSameLine(const pts &p1, const pts &p2, const pts &p3)
{

    float cross_product = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    return fabs(cross_product) < EPS;
}

Line createLineFromTwoPoints(const pts &p1, const pts &p2)
{
    Line line;

    if (fabs(p2.x - p1.x) < EPS)
    {
        line.isVertical = true;
        line.verticalX = p1.x;
    }
    else
    {
        line.m = (p2.y - p1.y) / (p2.x - p1.x);
        line.c = p1.y - line.m * p1.x;
        line.isVertical = false;
    }

    line.points.push_back(p1);
    line.points.push_back(p2);
    return line;
}

bool isPointOnLine(const pts &point, const Line &line)
{
    if (line.isVertical)
    {
        return fabs(point.x - line.verticalX) < EPS;
    }
    else
    {
        float expected_y = line.m * point.x + line.c;
        return fabs(point.y - expected_y) < EPS;
    }
}

vector<Line> findCollinearGroups(const vector<pts> &points)
{
    vector<Line> lines;
    vector<bool> used(points.size(), false);

    for (size_t i = 0; i < points.size(); i++)
    {
        if (used[i])
            continue;

        for (size_t j = i + 1; j < points.size(); j++)
        {
            if (used[j])
                continue;

            if (points[i] == points[j])
                continue;

            Line currentLine = createLineFromTwoPoints(points[i], points[j]);

            for (size_t k = 0; k < points.size(); k++)
            {
                if (k == i || k == j || used[k])
                    continue;

                if (isPointOnLine(points[k], currentLine))
                {
                    currentLine.points.push_back(points[k]);
                }
            }

            if (currentLine.points.size() >= 2)
            {
                for (const auto &p : currentLine.points)
                {
                    for (size_t idx = 0; idx < points.size(); idx++)
                    {
                        if (points[idx].x == p.x && points[idx].y == p.y &&
                            points[idx].originalIndex == p.originalIndex)
                        {
                            used[idx] = true;
                            break;
                        }
                    }
                }
                lines.push_back(currentLine);
                break;
            }
        }
    }

    return lines;
}

vector<pts> findIsolatedPoints(const vector<pts> &allPoints, const vector<Line> &lines)
{
    vector<pts> isolated;
    set<int> pointsInLines;

    for (const auto &line : lines)
    {
        for (const auto &point : line.points)
        {
            pointsInLines.insert(point.originalIndex);
        }
    }

    for (const auto &point : allPoints)
    {
        if (pointsInLines.find(point.originalIndex) == pointsInLines.end())
        {
            isolated.push_back(point);
        }
    }

    return isolated;
}

void analyzePoints(const vector<pts> &points)
{
    vector<pts> uniquePoints = removeDuplicates(points);
    vector<CoincidentGroup> coincidentGroups = findCoincidentGroups(points);

    cout << "=== POINT ANALYSIS ===" << endl;
    cout << "Original points: " << points.size() << endl;
    cout << "After removing duplicates: " << uniquePoints.size() << endl;

    cout << "\nAll unique points: ";
    for (const auto &p : uniquePoints)
    {
        p.print();
        cout << " ";
    }
    cout << endl;

    cout << "\n=== COINCIDENT GROUPS ===" << endl;
    vector<CoincidentGroup> actualCoincidentGroups;
    for (const auto &group : coincidentGroups)
    {
        if (group.originalIndices.size() > 1)
        {
            actualCoincidentGroups.push_back(group);
        }
    }

    if (actualCoincidentGroups.empty())
    {
        cout << "No coincident points found (all points are unique)" << endl;
    }
    else
    {
        for (size_t i = 0; i < actualCoincidentGroups.size(); i++)
        {
            cout << "Coincident Group " << (i + 1) << ": ";
            actualCoincidentGroups[i].print();
        }
    }

    if (uniquePoints.size() <= 1)
    {
        cout << "\nResult: All points are COINCIDENT (same point)" << endl;
        return;
    }

    if (uniquePoints.size() == 2)
    {
        cout << "\nResult: Only 2 unique points - they are COLLINEAR by definition" << endl;
        return;
    }

    vector<Line> collinearGroups = findCollinearGroups(uniquePoints);
    vector<pts> isolatedPoints = findIsolatedPoints(uniquePoints, collinearGroups);

    cout << "\n=== COLLINEAR GROUPS ===" << endl;
    if (collinearGroups.empty())
    {
        cout << "No collinear groups found (all points are isolated)" << endl;
    }
    else
    {
        for (size_t i = 0; i < collinearGroups.size(); i++)
        {
            cout << "Group " << (i + 1) << ": ";
            collinearGroups[i].print();
        }
    }

    cout << "\n=== ISOLATED POINTS ===" << endl;
    if (isolatedPoints.empty())
    {
        cout << "No isolated points (all points are part of collinear groups)" << endl;
    }
    else
    {
        for (const auto &p : isolatedPoints)
        {
            p.print();
            cout << " ";
        }
        cout << endl;
    }

    cout << "\n=== OVERALL RESULT ===" << endl;
    if (isolatedPoints.empty() && collinearGroups.size() == 1)
    {
        cout << "Result: ALL points are COLLINEAR" << endl;
    }
    else if (collinearGroups.empty())
    {
        cout << "Result: NO points are collinear" << endl;
    }
    else
    {
        cout << "Result: MIXED - Some points are collinear, some are not" << endl;
        cout << "- " << collinearGroups.size() << " collinear group(s) found" << endl;
        cout << "- " << isolatedPoints.size() << " isolated point(s) found" << endl;
    }

    if (!actualCoincidentGroups.empty())
    {
        cout << "- " << actualCoincidentGroups.size() << " coincident group(s) found" << endl;
    }
}

int main(int argc, char *argv[])
{
    string filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        cout << "Enter filename: ";
        cin >> filename;
    }
    vector<pts> points = readPoints(filename);

    if (points.empty())
    {
        cout << "No points found in file." << endl;
        return 1;
    }

    analyzePoints(points);

    return 0;
}