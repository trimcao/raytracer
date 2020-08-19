#pragma once

#include "doctest.h"
#include "Tuple.h"
#include "Util.h"

class Vector : public Tuple
{
public:
    Vector();
    Vector(float XIn, float YIn, float ZIn);

    float Magnitude();
    Vector Normalize();

    float Dot(const Vector &V);
    Vector Cross(const Vector &V);
};

inline Vector operator-(const Vector &A, const Vector &B)
{
    return Vector(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}