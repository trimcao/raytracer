#pragma once

#include "doctest.h"
#include "Tuple.h"

class Vector : public Tuple
{
public:
    Vector();
    Vector(float XIn, float YIn, float ZIn);

};

inline Vector operator-(const Vector& A, const Vector& B)
{
    return Vector(A.X-B.X, A.Y-B.Y, A.Z-B.Z);
}