#pragma once

#include "doctest.h"
#include "Tuple.h"
#include "Util.h"

class Vector : public Tuple
{
public:
    Vector();
    Vector(float XIn, float YIn, float ZIn);

    Vector operator-() const { return Vector(-X, -Y, -Z); }

    Vector operator*(float Scalar) const { return Vector(X * Scalar, Y * Scalar, Z * Scalar); }

    Vector operator/(float Scalar) const { return Vector(X / Scalar, Y / Scalar, Z / Scalar); }

    float Magnitude();
    Vector Normalize();

    float Dot(const Vector &V);
    Vector Cross(const Vector &V);
};

inline Vector operator-(const Vector &A, const Vector &B)
{
    return Vector(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

inline Vector operator+(const Vector &A, const Vector &B)
{
    return Vector(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

inline Vector operator*(float S, const Vector &B)
{
    return Vector(S * B.X, S * B.Y, S * B.Z);
}

std::ostream &operator<<(std::ostream &os, const Vector &V);