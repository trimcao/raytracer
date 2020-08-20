#pragma once

#include "doctest.h"
#include "Tuple.h"
#include <iostream>

class Color : public Tuple
{
public:
    Color();
    Color(float XIn, float YIn, float ZIn);

    Color operator-() const { return Color(-X, -Y, -Z); }

    Color operator*(float Scalar) const { return Color(X * Scalar, Y * Scalar, Z * Scalar); }

    Color operator/(float Scalar) const { return Color(X / Scalar, Y / Scalar, Z / Scalar); }
};

inline Color operator-(const Color &A, const Color &B)
{
    return Color(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
}

inline Color operator+(const Color &A, const Color &B)
{
    return Color(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}

inline Color operator*(const Color &A, const Color &B)
{
    return Color(A.X * B.X, A.Y * B.Y, A.Z * B.Z);
}

inline Color operator*(float S, const Color &B)
{
    return Color(S * B.X, S * B.Y, S * B.Z);
}

std::ostream &operator<<(std::ostream &os, const Color &P);