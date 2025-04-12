#ifndef VECTOR2D_H_INCLUDED
#define VECTOR2D_H_INCLUDED

struct Vector2D {
    float x, y;

    Vector2D();
    Vector2D(float _x, float _y);

    Vector2D operator+(const Vector2D &other) const;
    Vector2D operator-(const Vector2D &other) const;
    Vector2D& operator+=(const Vector2D &other);
    Vector2D operator*(const float variable) const;
    float length() const;
    Vector2D normalize() const;
};

#endif // VECTOR2D_H_INCLUDED
