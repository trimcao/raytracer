// #define DOCTEST_CONFIG_DISABLE
#include "include/Vector.h"

Vector::Vector() {}

Vector::Vector(float XIn, float YIn, float ZIn)
{
    X = XIn;
    Y = YIn;
    Z = ZIn;
    W = 0.f;
}

TEST_CASE("testing the Vector")
{
    Vector v = Vector(4.3f, -4.2f, 3.1f);
    CHECK(v == Tuple(4.3f, -4.2f, 3.1f, 0.f));
}

TEST_CASE("subtracting two vectors")
{
    Vector A = Vector(3.f, 2.f, 1.f);
    Vector B = Vector(5.f, 6.f, 7.f);
    CHECK((A - B) == Vector(-2.f, -4.f, -6.f));
}