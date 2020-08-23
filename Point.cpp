// #define DOCTEST_CONFIG_DISABLE
#include "include/doctest.h"
#include <iostream>
#include "include/Point.h"

Point::Point() {}

Point::Point(float X, float Y, float Z) : Matrix(4, 1)
{
    this->Set(0, 0, X);
    this->Set(1, 0, Y);
    this->Set(2, 0, Z);
    this->Set(3, 0, 1.f);
}

std::ostream &operator<<(std::ostream &os, const Point &P)
{
    os << "Point{ " << P.X() << ", " << P.Y() << ", " << P.Z() << " }";
    return os;
}

TEST_CASE("testing the Point")
{
    Point v = Point(4.3f, -4.2f, 3.1f);
    CHECK(v == Matrix(4.3f, -4.2f, 3.1f, 1.f));
}

TEST_CASE("multiplying a point with a scalar")
{
    Point v = Point(1.f, -1.f, 3.f);
    Matrix M = Matrix(4, 1);
    M(0, 0) = 2.f;
    M(1, 0) = -2.f;
    M(2, 0) = 6.f;
    M(3, 0) = 1.f;

    CHECK(v * 2 == M);
}

TEST_CASE("subtracting two points")
{
    Point A = Point(3.f, 2.f, 1.f);
    Point B = Point(5.f, 6.f, 7.f);
    CHECK((A - B) == Vector(-2.f, -4.f, -6.f));
}

TEST_CASE("add one point to one vector")
{
    Point A = Point(3.f, 2.f, 1.f);
    Vector B = Vector(5.f, 6.f, 7.f);
    CHECK((A + B) == Point(8.f, 8.f, 8.f));
    CHECK((B + A) == Point(8.f, 8.f, 8.f));
}

TEST_CASE("subtracting a vector from a point")
{
    Point A = Point(3.f, 2.f, 1.f);
    Vector B = Vector(5.f, 6.f, 7.f);
    CHECK((A - B) == Point(-2.f, -4.f, -6.f));
}