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

};

inline bool operator==(const Tuple& LHS, const Tuple& RHS)
{
    return (equal(LHS.X, RHS.X) && equal(LHS.Y, RHS.Y) && equal(LHS.Z, RHS.Z) && equal(LHS.W, RHS.W));
}

inline bool operator!=(const Tuple& LHS, const Tuple& RHS)
{
    return ( !(LHS == RHS) );
}

