#ifndef POINT_H
#define POINT_H

struct Point {
    double x, y;
    
    Point(double x = 0, double y = 0);
    
    Point operator-(const Point& other) const;
    Point operator+(const Point& other) const;
    double cross(const Point& other) const;
    double dot(const Point& other) const;
};

#endif