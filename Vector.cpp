// #define DOCTEST_CONFIG_DISABLE
#include "include/doctest.h"
#include "include/Tuple.h"
#include "include/Util.h"
#include <cmath>
#include <iostream>
#include "include/Vector.h"

Vector::Vector() {}

Vector::Vector(float XIn, float YIn, float ZIn)
{
    X = XIn;
    Y = YIn;
    Z = ZIn;
    W = 0.f;
}

float Vector::Magnitude()
{
    return std::sqrt(pow(X, 2.f) + pow(Y, 2.f) + pow(Z, 2.f));
}

Vector Vector::Normalize()
{
    float Mag = this->Magnitude();
    return Vector(X / Mag, Y / Mag, Z / Mag);
}

float Vector::Dot(const Vector &V)
{
    return (X * V.X + Y * V.Y + Z * V.Z);
}

Vector Vector::Cross(const Vector &V)
{
    return Vector(Y * V.Z - Z * V.Y,
                  Z * V.X - X * V.Z,
                  X * V.Y - Y * V.X);
}

std::ostream &operator<<(std::ostream &os, const Vector &V)
{
    os << "Vector{ " << V.X << ", " << V.Y << ", " << V.Z << " }";
    return os;
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

TEST_CASE("computing the magnitude of a vector")
{
    Vector A = Vector(1.f, 0.f, 0.f);
    Vector B = Vector(0.f, 1.f, 0.f);
    Vector C = Vector(0.f, 0.f, 1.f);
    Vector D = Vector(1.f, 2.f, 3.f);
    Vector E = Vector(-1.f, -2.f, -3.f);

    CHECK(Util::Equal(A.Magnitude(), 1.0));
    CHECK(Util::Equal(B.Magnitude(), 1.0));
    CHECK(Util::Equal(C.Magnitude(), 1.0));
    CHECK(Util::Equal(D.Magnitude(), std::sqrt(14)));
    CHECK(Util::Equal(E.Magnitude(), std::sqrt(14)));
}

TEST_CASE("computing the normalization of a vector")
{
    Vector A = Vector(4.f, 0.f, 0.f);
    Vector D = Vector(1.f, 2.f, 3.f);
    Vector E = Vector(-1.f, -2.f, -3.f);

    CHECK(A.Normalize() == Vector(1.f, 0.f, 0.f));
    CHECK(D.Normalize() == Vector(0.26726f, 0.53452f, 0.80178f));
    CHECK(Util::Equal(D.Normalize().Magnitude(), 1.f));
}

TEST_CASE("dot product of two vectors")
{
    Vector A = Vector(1.f, 2.f, 3.f);
    Vector B = Vector(2.f, 3.f, 4.f);

    CHECK(Util::Equal(A.Dot(B), 20.f));
    CHECK(Util::Equal(B.Dot(A), 20.f));
}

TEST_CASE("cross product of two vectors")
{
    Vector A = Vector(1.f, 2.f, 3.f);
    Vector B = Vector(2.f, 3.f, 4.f);

    CHECK(A.Cross(B) == Vector(-1.f, 2.f, -1.f));
    CHECK(B.Cross(A) == Vector(1.f, -2.f, 1.f));
}