#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Point.h"

Point::Point() : Matrix(4, 1)
{
    this->Set(3, 0, 1.);
}

Point::Point(int NumRows, int NumCols) : Matrix(4, 1)
{
    if (NumRows != 4 && NumCols != 1)
        throw std::invalid_argument("size of a Point must be 4x1");
}

Point::Point(double X, double Y, double Z) : Matrix(4, 1)
{
    this->Set(0, 0, X);
    this->Set(1, 0, Y);
    this->Set(2, 0, Z);
    this->Set(3, 0, 1.);
}

Point::Point(Matrix &M)
{
    if (M.GetNumRows() != 4 && M.GetNumCols() != 1)
        throw std::invalid_argument("size of a Point must be 4x1");
    if (!Util::Equal(M.At(3,0), 1.))
        throw std::invalid_argument("W value must be 1.0");

    this->Set(0, 0, M.At(0, 0));
    this->Set(1, 0, M.At(1, 0));
    this->Set(2, 0, M.At(2, 0));
    this->Set(3, 0, M.At(3, 0));
}

std::ostream &operator<<(std::ostream &os, const Point &P)
{
    os << "Point{ " << P.X() << ", " << P.Y() << ", " << P.Z() << " }";
    return os;
}

TEST_CASE("testing the Point")
{
    Point v = Point(4.3, -4.2, 3.1);
    CHECK(v == Matrix(4.3, -4.2, 3.1, 1.));
}

TEST_CASE("multiplying a point with a scalar")
{
    Point v = Point(1., -1., 3.);
    Matrix M = Matrix(4, 1);
    M(0, 0) = 2.;
    M(1, 0) = -2.;
    M(2, 0) = 6.;
    M(3, 0) = 1.;

    CHECK(v * 2 == M);
}

TEST_CASE("subtracting two points")
{
    Point A = Point(3., 2., 1.);
    Point B = Point(5., 6., 7.);
    CHECK((A - B) == Vector(-2., -4., -6.));
}

TEST_CASE("add one point to one vector")
{
    Point A = Point(3., 2., 1.);
    Vector B = Vector(5., 6., 7.);
    CHECK((A + B) == Point(8., 8., 8.));
    CHECK((B + A) == Point(8., 8., 8.));
}

TEST_CASE("subtracting a vector from a point")
{
    Point A = Point(3., 2., 1.);
    Vector B = Vector(5., 6., 7.);
    CHECK((A - B) == Point(-2., -4., -6.));
}