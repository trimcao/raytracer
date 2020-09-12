// #define DOCTEST_CONFIG_DISABLE
#include "include/Matrix.h"
#include "include/Util.h"
#include <cmath>
#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Vector.h"

Vector::Vector() {}

Vector::Vector(int NumRows, int NumCols) : Matrix(4, 1)
{
    if (NumRows != 4 && NumCols != 1)
        throw std::invalid_argument("size of a Vector must be 4x1");
}

Vector::Vector(double X, double Y, double Z) : Matrix(4, 1)
{
    this->Set(0, 0, X);
    this->Set(1, 0, Y);
    this->Set(2, 0, Z);
    this->Set(3, 0, 0.f);
}

Vector::Vector(Matrix &M)
{
    if (M.GetNumRows() != 4 && M.GetNumCols() != 1)
        throw std::invalid_argument("size of a Vector must be 4x1");
    if (!Util::Equal(M.At(3,0), 1.f))
        throw std::invalid_argument("W value must be 1.0");

    this->Set(0, 0, M.At(0, 0));
    this->Set(1, 0, M.At(1, 0));
    this->Set(2, 0, M.At(2, 0));
    this->Set(3, 0, M.At(3, 0));
}

Vector::Vector(Matrix &&M) : Vector(M)
{
}

double Vector::Magnitude()
{
    return std::sqrt(pow(X(), 2.f) + pow(Y(), 2.f) + pow(Z(), 2.f));
}

Vector Vector::Normalize()
{
    double Mag = this->Magnitude();
    return Vector(X()/ Mag, Y() / Mag, Z() / Mag);
}

double Vector::Dot(const Vector &V)
{
    return (X() * V.X() + Y() * V.Y() + Z() * V.Z());
}

Vector Vector::Cross(const Vector &V)
{
    return Vector(Y() * V.Z() - Z() * V.Y(),
                  Z() * V.X() - X() * V.Z(),
                  X() * V.Y() - Y() * V.X());
}

Vector Vector::Reflect(const Vector &N)
{
    return *this - (N * 2 * this->Dot(N));
}

std::ostream &operator<<(std::ostream &os, const Vector &V)
{
    os << "Vector{ " << V.X() << ", " << V.Y() << ", " << V.Z() << " }";
    return os;
}

TEST_CASE("testing the Vector")
{
    Vector v = Vector(4.3, -4.2, 3.1f);
    CHECK(v == Matrix(4.3, -4.2, 3.1, 0.f));
}

TEST_CASE("subtracting two vectors")
{
    Vector A = Vector(3., 2., 1.f);
    Vector B = Vector(5., 6., 7.f);
    CHECK((A - B) == Vector(-2., -4., -6.f));
}

TEST_CASE("computing the magnitude of a vector")
{
    Vector A = Vector(1., 0., 0.f);
    Vector B = Vector(0., 1., 0.f);
    Vector C = Vector(0., 0., 1.f);
    Vector D = Vector(1., 2., 3.f);
    Vector E = Vector(-1., -2., -3.f);

    CHECK(Util::Equal(A.Magnitude(), 1.0));
    CHECK(Util::Equal(B.Magnitude(), 1.0));
    CHECK(Util::Equal(C.Magnitude(), 1.0));
    CHECK(Util::Equal(D.Magnitude(), std::sqrt(14)));
    CHECK(Util::Equal(E.Magnitude(), std::sqrt(14)));
}

TEST_CASE("computing the normalization of a vector")
{
    Vector A = Vector(4., 0., 0.f);
    Vector D = Vector(1., 2., 3.f);
    Vector E = Vector(-1., -2., -3.f);

    CHECK(A.Normalize() == Vector(1., 0., 0.f));
    CHECK(D.Normalize() == Vector(0.26726, 0.53452, 0.80178f));
    CHECK(Util::Equal(D.Normalize().Magnitude(), 1.f));
}

TEST_CASE("dot product of two vectors")
{
    Vector A = Vector(1., 2., 3.f);
    Vector B = Vector(2., 3., 4.f);

    CHECK(Util::Equal(A.Dot(B), 20.f));
    CHECK(Util::Equal(B.Dot(A), 20.f));
}

TEST_CASE("cross product of two vectors")
{
    Vector A = Vector(1., 2., 3.f);
    Vector B = Vector(2., 3., 4.f);

    CHECK(A.Cross(B) == Vector(-1., 2., -1.f));
    CHECK(B.Cross(A) == Vector(1., -2., 1.f));
}

TEST_CASE("Reflecting a vector approaching at 45 degrees")
{
    Vector V(1., -1., 0.f);
    Vector N(0., 1., 0.f);
    auto R = V.Reflect(N);
    CHECK(R == Vector(1., 1., 0.f));
}

TEST_CASE("Reflecting a vector off a slanted surface")
{
    Vector V(0., -1., 0.f);
    Vector N(std::sqrt(2.f)/2, std::sqrt(2.f)/2, 0.f);
    auto R = V.Reflect(N);
    CHECK(R == Vector(1., 0., 0.f));
}