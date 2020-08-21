#pragma once

#include "doctest.h"
#include "Tuple.h"
#include <iostream>
#include <string>
#include <vector>

class Color : public Tuple
{
public:
    Color();
    Color(float R, float G, float B);

    Color operator-() const { return Color(-X, -Y, -Z); }

    Color operator*(float Scalar) const { return Color(X * Scalar, Y * Scalar, Z * Scalar); }

    Color operator/(float Scalar) const { return Color(X / Scalar, Y / Scalar, Z / Scalar); }

    inline float R() { return X; };
    inline float G() { return Y; };
    inline float B() { return Z; };

    // convert a Color object to a PPM int values
    std::vector<int> ToPPMVal(int MaxColorValue);
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