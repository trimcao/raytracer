#pragma once

#include "doctest.h"
#include "Tuple.h"
#include "Vector.h"
#include <iostream>

class Point : public Tuple
{
public:
    Point();
    Point(float XIn, float YIn, float ZIn);
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

std::ostream &operator<<(std::ostream &os, const Point &P);