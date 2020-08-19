#include "doctest.h"
#include "Util.h"

class Tuple
{

public:

    float X, Y, Z, W;

    Tuple();

    Tuple(float X, float Y, float Z, float W);

};

inline bool operator==(const Tuple& LHS, const Tuple& RHS)
{
    return (equal(LHS.X, RHS.X) && equal(LHS.Y, RHS.Y) && equal(LHS.Z, RHS.Z) && equal(LHS.W, RHS.W));
}

inline bool operator!=(const Tuple& LHS, const Tuple& RHS)
{
    return ( !(LHS == RHS) );
}

