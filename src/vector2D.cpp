#include "vector2D.h"

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y) {}

Vector2D Vector2D::operator+(const Vector2D &other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator-(const Vector2D &other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D& Vector2D::operator+=(const Vector2D &other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D Vector2D::operator*(const float variable) const {
    return Vector2D(x * variable, y * variable);
}

float Vector2D::length() const {
    return sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    float len = length();
    return len == 0 ? Vector2D(0, 0) : Vector2D(x / len, y / len);
}
