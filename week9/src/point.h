#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
    double x, y;
    int index;
    
    Point(double x = 0, double y = 0, int index = -1);
    
    Point operator-(const Point& other) const;
    Point operator+(const Point& other) const;
    bool operator<(const Point& other) const;
    bool operator==(const Point& other) const;
    
    double cross(const Point& other) const;
    double dot(const Point& other) const;
    
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

double orientation(const Point& p, const Point& q, const Point& r);
bool isLeftTurn(const Point& p, const Point& q, const Point& r);

#endif