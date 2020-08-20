#pragma once

#include "doctest.h"
#include "Util.h"

class Tuple
{

public:
    float X = 0.f;
    float Y = 0.f;
    float Z = 0.f;
    float W = 0.f;

    Tuple();

    Tuple(float XIn, float YIn, float ZIn, float WIn);

    Tuple operator-() const { return Tuple(-X, -Y, -Z, -W); }

    Tuple operator*(float Scalar) const { return Tuple(X * Scalar, Y * Scalar, Z * Scalar, W * Scalar); }

    Tuple operator/(float Scalar) const { return Tuple(X / Scalar, Y / Scalar, Z / Scalar, W / Scalar); }
};

inline bool operator==(const Tuple &LHS, const Tuple &RHS)
{
    return (Util::Equal(LHS.X, RHS.X) && Util::Equal(LHS.Y, RHS.Y) && Util::Equal(LHS.Z, RHS.Z) && Util::Equal(LHS.W, RHS.W));
}

inline bool operator!=(const Tuple &LHS, const Tuple &RHS)
{
    return (!(LHS == RHS));
}

std::ostream &operator<<(std::ostream &os, const Tuple &T);
