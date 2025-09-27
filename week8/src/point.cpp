#include "point.h"

Point::Point(double x, double y) : x(x), y(y) {}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

double Point::cross(const Point& other) const {
    return x * other.y - y * other.x;
}

double Point::dot(const Point& other) const {
    return x * other.x + y * other.y;
}