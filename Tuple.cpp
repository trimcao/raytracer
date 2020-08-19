#include "include/Tuple.h"

Tuple::Tuple()
{

}

Tuple::Tuple(float XIn, float YIn, float ZIn, float WIn)
{
    X = XIn;
    Y = YIn;
    Z = ZIn;
    W = WIn;
}

TEST_CASE("test comparison tuple")
{
    Tuple A = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple B = Tuple(1.0, 1.5, 0.5, 1.2);
    Tuple C = Tuple(1.0, 1.5, 0.5, 1.15);

    CHECK(A == B);
    CHECK(B == A);
    CHECK(A != C);
    CHECK(C != B);
}

TEST_CASE("test negate tuple")
{
    Tuple A = Tuple(1.f, -2.f, 3.f, -4.f);
    Tuple B = -A;

    CHECK(B == Tuple(-1.f, 2.f, -3.f, 4.f));
}