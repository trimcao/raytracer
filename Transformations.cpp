#include "include/Transformations.h"
#include <cmath>

Matrix Transformations::ViewTransform(Point &From, Point &To, Vector &Up)
{
    auto Forward = (To - From).Normalize();
    auto Left = Forward.Cross(Up.Normalize());
    auto TrueUp = Left.Cross(Forward);

    auto Orient = Matrix::Identity(4);
    Orient(0, 0) = Left.X();
    Orient(0, 1) = Left.Y();
    Orient(0, 2) = Left.Z();
    Orient(1, 0) = TrueUp.X();
    Orient(1, 1) = TrueUp.Y();
    Orient(1, 2) = TrueUp.Z();
    Orient(2, 0) = -Forward.X();
    Orient(2, 1) = -Forward.Y();
    Orient(2, 2) = -Forward.Z();

    return Orient.Mul(Transformations::Translation(-From.X(), -From.Y(), -From.Z()));
}

Matrix Transformations::ViewTransform(Point &&From, Point &&To, Vector &&Up)
{
    return ViewTransform(From, To, Up);
}

Matrix Transformations::Translation(double X, double Y, double Z)
{
    Matrix Res = Matrix::Identity(4);
    Res(0, 3) = X;
    Res(1, 3) = Y;
    Res(2, 3) = Z;
    return Res;
}

Matrix Transformations::Scaling(double X, double Y, double Z)
{
    Matrix Res = Matrix::Identity(4);
    Res(0, 0) = X;
    Res(1, 1) = Y;
    Res(2, 2) = Z;
    return Res;
}

Matrix Transformations::RotationX(double Rad)
{
    Matrix Res = Matrix::Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(1, 1) = Cos;
    Res(1, 2) = -Sin;
    Res(2, 1) = Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Transformations::RotationY(double Rad)
{
    Matrix Res = Matrix::Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 2) = Sin;
    Res(2, 0) = -Sin;
    Res(2, 2) = Cos;
    return Res;
}

Matrix Transformations::RotationZ(double Rad)
{
    Matrix Res = Matrix::Identity(4);
    double Sin = std::sin(Rad);
    double Cos = std::cos(Rad);
    Res(0, 0) = Cos;
    Res(0, 1) = -Sin;
    Res(1, 0) = Sin;
    Res(1, 1) = Cos;
    return Res;
}

Matrix Transformations::Shearing(double XY, double XZ, double YX, double YZ, double ZX, double ZY)
{
    Matrix Res = Matrix::Identity(4);
    Res(0, 1) = XY;
    Res(0, 2) = XZ;
    Res(1, 0) = YX;
    Res(1, 2) = YZ;
    Res(2, 0) = ZX;
    Res(2, 1) = ZY;
    return Res;
}

TEST_CASE("The transformation matrix for the default orientation")
{
    Point From(0., 0., 0.);
    Point To(0., 0., -1.);
    Vector Up(0., 1., 0.);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Identity(4));
}

TEST_CASE("The transformation matrix looking in positive z direction")
{
    Point From(0., 0., 0.);
    Point To(0., 0., 1.);
    Vector Up(0., 1., 0.);

    CHECK(Transformations::ViewTransform(From, To, Up) == Transformations::Scaling(-1., 1., -1.));
}

TEST_CASE("The transformation matrix moves the world")
{
    Point From(0., 0., 8.);
    Point To(0., 0., 0.);
    Vector Up(0., 1., 0.);

    CHECK(Transformations::ViewTransform(From, To, Up) == Transformations::Translation(0., 0., -8.));
}

TEST_CASE("An arbitrary view transformation")
{
    Point From(1., 3., 2.);
    Point To(4., -2., 8.);
    Vector Up(1., 1., 0.);

    auto E = Matrix::Identity(4);
    E(0, 0) = -0.50709; E(0, 1) = 0.50709; E(0, 2) = 0.67612; E(0, 3) = -2.36643;
    E(1, 0) = 0.76772; E(1, 1) = 0.60609; E(1, 2) = 0.12122; E(1, 3) = -2.82843;
    E(2, 0) = -0.35857; E(2, 1) = 0.59761; E(2, 2) = -0.71714; E(2, 3) = 0.;
    E(3, 0) = 0.; E(3, 1) = 0.; E(3, 2) = 0.; E(3, 3) = 1.;

    CHECK(Transformations::ViewTransform(From, To, Up) == E);
}