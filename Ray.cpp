#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include "include/Ray.h"
#include "include/Sphere.h"

Ray::Ray() {}

Ray::Ray(Point &&O, Vector &&D)
{
    Origin = O;
    Direction = D;
}

Ray::Ray(Point &O, Vector &&D)
{
    Origin = O;
    Direction = D;
}

Ray::Ray(Point &O, Vector &D)
{
    Origin = O;
    Direction = D;
}

Point Ray::GetOrigin() const
{
    return Origin;
}

Vector Ray::GetDirection() const
{
    return Direction;
}

Point Ray::Position(double T)
{
    return Origin + Direction * T;
}

Ray Ray::Transform(Matrix &M) const
{
    return Ray(M.Mul(Origin), M.Mul(Direction));
}

Ray Ray::Transform(Matrix &&M) const
{
    return Ray(M.Mul(Origin), M.Mul(Direction));
}

TEST_CASE("Creating and querying a Ray")
{
    Ray R(Point(1., 2., 3.f), Vector(4., 5., 6.f));

    CHECK(R.GetOrigin() == Point(1., 2., 3.f));
    CHECK(R.GetDirection() == Vector(4., 5., 6.f));
}

TEST_CASE("Computing a point from a distance")
{
    Ray R(Point(2., 3., 4.f), Vector(1., 0., 0.f));

    CHECK(R.Position(0.f) == Point(2., 3., 4.f));
    CHECK(R.Position(1.f) == Point(3., 3., 4.f));
    CHECK(R.Position(-1.f) == Point(1., 3., 4.f));
    CHECK(R.Position(2.5f) == Point(4.5, 3., 4.f));
}

TEST_CASE("Translating a ray")
{
    Ray R(Point(1., 2., 3.f), Vector(0., 1., 0.f));
    auto M = Matrix::Translation(3., 4., 5.f);
    auto R2 = R.Transform(M);

    CHECK(R2.GetOrigin() == Point(4., 6., 8.f));
    CHECK(R2.GetDirection() == Vector(0., 1., 0.f));
}

TEST_CASE("Scaling a ray")
{
    Ray R(Point(1., 2., 3.f), Vector(0., 1., 0.f));
    auto M = Matrix::Scaling(2., 3., 4.f);
    auto R2 = R.Transform(M);

    CHECK(R2.GetOrigin() == Point(2., 6., 12.f));
    CHECK(R2.GetDirection() == Vector(0., 3., 0.f));
}