#pragma once

#include "doctest.h"
#include <vector>
#include "Util.h"

class Color
{
public:
    double R, G, B;

    Color();
    Color(double R, double G, double B);

    Color operator-() const { return Color(-R, -G, -B); }

    Color operator*(double Scalar) const { return Color(R * Scalar, G * Scalar, B * Scalar); }

    Color operator/(double Scalar) const { return Color(R / Scalar, G / Scalar, B / Scalar); }

    // convert a Color object to a PPM int values
    std::vector<int> ToPPMVal(int MaxColorValue);
};

inline Color operator-(const Color &A, const Color &B)
{
    return Color(A.R - B.R, A.G - B.G, A.B - B.B);
}

inline Color operator+(const Color &A, const Color &B)
{
    return Color(A.R + B.R, A.G + B.G, A.B + B.B);
}

inline Color operator*(const Color &A, const Color &B)
{
    return Color(A.R * B.R, A.G * B.G, A.B * B.B);
}

inline Color operator*(double S, const Color &B)
{
    return Color(S * B.R, S * B.G, S * B.B);
}

inline bool operator==(const Color &LHS, const Color &RHS)
{
    return (Util::Equal(LHS.R, RHS.R) && Util::Equal(LHS.G, RHS.G) && Util::Equal(LHS.B, RHS.B));
}

inline bool operator!=(const Color &LHS, const Color &RHS)
{
    return (!(LHS == RHS));
}

std::ostream &operator<<(std::ostream &os, const Color &P);