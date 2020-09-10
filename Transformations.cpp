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
    Point From(0.f, 0.f, 0.f);
    Point To(0.f, 0.f, -1.f);
    Vector Up(0.f, 1.f, 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Identity(4));
}

TEST_CASE("The transformation matrix looking in positive z direction")
{
    Point From(0.f, 0.f, 0.f);
    Point To(0.f, 0.f, 1.f);
    Vector Up(0.f, 1.f, 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Scaling(-1.f, 1.f, -1.f));
}

TEST_CASE("The transformation matrix moves the world")
{
    Point From(0.f, 0.f, 8.f);
    Point To(0.f, 0.f, 0.f);
    Vector Up(0.f, 1.f, 0.f);

    CHECK(Transformations::ViewTransform(From, To, Up) == Matrix::Translation(0.f, 0.f, -8.f));
}

TEST_CASE("An arbitrary view transformation")
{
    Point From(1.f, 3.f, 2.f);
    Point To(4.f, -2.f, 8.f);
    Vector Up(1.f, 1.f, 0.f);

    auto E = Matrix::Identity(4);
    E(0, 0) = -0.50709f; E(0, 1) = 0.50709f; E(0, 2) = 0.67612f; E(0, 3) = -2.36643f;
    E(1, 0) = 0.76772f; E(1, 1) = 0.60609f; E(1, 2) = 0.12122f; E(1, 3) = -2.82843f;
    E(2, 0) = -0.35857f; E(2, 1) = 0.59761f; E(2, 2) = -0.71714f; E(2, 3) = 0.f;
    E(3, 0) = 0.f; E(3, 1) = 0.f; E(3, 2) = 0.f; E(3, 3) = 1.f;

    CHECK(Transformations::ViewTransform(From, To, Up) == E);
}