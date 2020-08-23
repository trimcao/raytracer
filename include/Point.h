#pragma once

#include "Matrix.h"
#include "Tuple.h"
#include "Vector.h"

class Point : public Tuple
{
public:
    Point();
    Point(float XIn, float YIn, float ZIn);

    Point operator-() const { return Point(-X, -Y, -Z); }

    Point operator*(float Scalar) const { return Point(X * Scalar, Y * Scalar, Z * Scalar); }

    Point operator/(float Scalar) const { return Point(X / Scalar, Y / Scalar, Z / Scalar); }
};

inline Vector operator-(const Point &A, const Point &B)
{
    return Vector(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

inline Point operator+(const Point &A, const Vector &B)
{
    return Point(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

inline Point operator+(const Vector &A, const Point &B)
{
    return Point(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

inline Point operator-(const Point &A, const Vector &B)
{
    return Point(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

inline Point operator*(float S, const Point &B)
{
    return Point(S * B.X, S * B.Y, S * B.Z);
}

std::ostream &operator<<(std::ostream &os, const Point &P);