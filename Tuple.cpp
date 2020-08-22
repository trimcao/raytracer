#include "include/doctest.h"
#include <iostream>
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

std::ostream &operator<<(std::ostream &os, const Tuple &T)
{
    os << "Tuple{ " << T.X << ", " << T.Y << ", " << T.Z << ", " << T.W << " }";
    return os;
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

TEST_CASE("negating a tuple")
{
    Tuple A = Tuple(1.f, -2.f, 3.f, -4.f);
    Tuple B = -A;

    CHECK(B == Tuple(-1.f, 2.f, -3.f, 4.f));
}

TEST_CASE("multiplying a tuple by a scalar")
{
    Tuple A = Tuple(1.f, -2.f, 3.f, -4.f);

    CHECK(A * 3.5 == Tuple(3.5f, -7.f, 10.5f, -14.f));
    CHECK(A * 0.5 == Tuple(0.5f, -1.f, 1.5f, -2.f));
}

TEST_CASE("dividing a tuple by a scalar")
{
    Tuple A = Tuple(1.f, -2.f, 3.f, -4.f);

    CHECK(A / 2 == Tuple(0.5f, -1.f, 1.5f, -2.f));
}