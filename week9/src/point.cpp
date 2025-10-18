#include "point.h"
#include <cmath>

Point::Point(double x, double y, int index) : x(x), y(y), index(index) {}

Point Point::operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
}

Point Point::operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
}

bool Point::operator<(const Point& other) const {
    if (std::abs(y - other.y) < 1e-9) {
        return x < other.x;
    }
    return y > other.y;
}

bool Point::operator==(const Point& other) const {
    return std::abs(x - other.x) < 1e-9 && std::abs(y - other.y) < 1e-9;
}

double Point::cross(const Point& other) const {
    return x * other.y - y * other.x;
}

double Point::dot(const Point& other) const {
    return x * other.x + y * other.y;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

double orientation(const Point& p, const Point& q, const Point& r) {
    return (q - p).cross(r - p);
}

bool isLeftTurn(const Point& p, const Point& q, const Point& r) {
    return orientation(p, q, r) > 1e-9;
}