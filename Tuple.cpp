#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Tuple.h"

Tuple::Tuple()
{
}

Tuple::Tuple(int NumRows, int NumCols) : Matrix(4, 1)
{
    if (NumRows != 4 && NumCols != 1)
        throw std::invalid_argument("size of a Tuple must be 4x1");
}

Tuple::Tuple(double X, double Y, double Z, double W) : Matrix(4, 1)
{
    this->Set(0, 0, X);
    this->Set(1, 0, Y);
    this->Set(2, 0, Z);
    this->Set(3, 0, W);
}

std::ostream &operator<<(std::ostream &os, const Tuple &T)
{
    os << "Tuple{ " << T.X() << ", " << T.Y() << ", " << T.Z() << ", " << T.W() << " }";
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
    Tuple A = Tuple(1., -2., 3., -4.);
    Tuple B = -A;

    CHECK(B == Tuple(-1., 2., -3., 4.));
}

TEST_CASE("multiplying a tuple by a scalar")
{
    Tuple A = Tuple(1., -2., 3., -4.);

    CHECK(A * 3.5 == Tuple(3.5, -7., 10.5, -14.));
    CHECK(A * 0.5 == Tuple(0.5, -1., 1.5, -2.));
}

TEST_CASE("dividing a tuple by a scalar")
{
    Tuple A = Tuple(1., -2., 3., -4.);

    CHECK(A / 2 == Tuple(0.5, -1., 1.5, -2.));
}