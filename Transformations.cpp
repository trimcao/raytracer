#include "include/Transformations.h"

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

    return Orient.Mul(Matrix::Translation(-From.X(), -From.Y(), -From.Z()));
}

Matrix Transformations::ViewTransform(Point &&From, Point &&To, Vector &&Up)
{
    return ViewTransform(From, To, Up);
}

TEST_CASE("The transformation matrix for the default orientation")
{
    Point From(0., 0., 0.f);
    Point To(0., 0., -1.f);
    Vector Up(0., 1., 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Identity(4));
}

TEST_CASE("The transformation matrix looking in positive z direction")
{
    Point From(0., 0., 0.f);
    Point To(0., 0., 1.f);
    Vector Up(0., 1., 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Scaling(-1., 1., -1.f));
}

TEST_CASE("The transformation matrix moves the world")
{
    Point From(0., 0., 8.f);
    Point To(0., 0., 0.f);
    Vector Up(0., 1., 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Translation(0., 0., -8.f));
}

TEST_CASE("An arbitrary view transformation")
{
    Point From(1., 3., 2.f);
    Point To(4., -2., 8.f);
    Vector Up(1., 1., 0.f);

    auto E = Matrix::Identity(4);
    E(0, 0) = -0.50709; E(0, 1) = 0.50709; E(0, 2) = 0.67612; E(0, 3) = -2.36643;
    E(1, 0) = 0.76772; E(1, 1) = 0.60609; E(1, 2) = 0.12122; E(1, 3) = -2.82843;
    E(2, 0) = -0.35857; E(2, 1) = 0.59761; E(2, 2) = -0.71714; E(2, 3) = 0.;
    E(3, 0) = 0.; E(3, 1) = 0.; E(3, 2) = 0.; E(3, 3) = 1.;

    CHECK(Transformations::ViewTransform(From, To, Up) == E);
}