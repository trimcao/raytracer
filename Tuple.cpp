#include "include/Tuple.h"

Tuple::Tuple()
{

}

Tuple::Tuple(float X, float Y, float Z, float W)
{
    this->X = X;
    this->Y = Y;
    this->Z = Z;
    this->W = W;
}

TEST_CASE("test comparison Tuple")
{
    Tuple A = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple B = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple C = Tuple(1.0, 1.5, 0.5, 1.15);

    CHECK(A == B);
    CHECK(B == A);
    CHECK(A != C);
    CHECK(C != B);
}