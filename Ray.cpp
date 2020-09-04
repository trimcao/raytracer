#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include "include/Ray.h"
#include "include/Sphere.h"
#include "include/Intersection.h"

Ray::Ray() {}

Ray::Ray(Point &&O, Vector &&D)
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

Point Ray::Position(float T)
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

std::vector<Intersection> Intersect(const Ray &R, const Sphere &S)
{
    std::vector<Intersection> Intersections;

    auto TransformedRay = R.Transform(S.GetTransform().Inverse());

    // assume the origin of Sphere is always (0.f, 0.f, 0.f)
    Vector SphereToRay = TransformedRay.GetOrigin() - Point(0.f, 0.f, 0.f);
    float A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
    float B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
    float C = SphereToRay.Dot(SphereToRay) - 1.f;

    float Discriminant = B * B - 4 * A * C;

    if (Discriminant >= 0.f)
    {
        Intersections.push_back(Intersection((-B - std::sqrt(Discriminant)) / (2 * A), S));
        Intersections.push_back(Intersection((-B + std::sqrt(Discriminant)) / (2 * A), S));
    }

    return Intersections;
}

TEST_CASE("Creating and querying a Ray")
{
    Ray R(Point(1.f, 2.f, 3.f), Vector(4.f, 5.f, 6.f));

    CHECK(R.GetOrigin() == Point(1.f, 2.f, 3.f));
    CHECK(R.GetDirection() == Vector(4.f, 5.f, 6.f));
}

TEST_CASE("Computing a point from a distance")
{
    Ray R(Point(2.f, 3.f, 4.f), Vector(1.f, 0.f, 0.f));

    CHECK(R.Position(0.f) == Point(2.f, 3.f, 4.f));
    CHECK(R.Position(1.f) == Point(3.f, 3.f, 4.f));
    CHECK(R.Position(-1.f) == Point(1.f, 3.f, 4.f));
    CHECK(R.Position(2.5f) == Point(4.5f, 3.f, 4.f));
}

TEST_CASE("A ray intersects a sphere at two points")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.0));
    CHECK(Util::Equal(XS[1].GetT(), 6.0));
}

TEST_CASE("A ray misses a sphere")
{
    Ray R(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray originates inside a sphere")
{
    Ray R(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -1.0));
    CHECK(Util::Equal(XS[1].GetT(), 1.0));
}

TEST_CASE("A sphere is behind a ray")
{
    Ray R(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -6.0));
    CHECK(Util::Equal(XS[1].GetT(), -4.0));
}

TEST_CASE("Translating a ray")
{
    Ray R(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f));
    auto M = Matrix::Translation(3.f, 4.f, 5.f);
    auto R2 = R.Transform(M);

    CHECK(R2.GetOrigin() == Point(4.f, 6.f, 8.f));
    CHECK(R2.GetDirection() == Vector(0.f, 1.f, 0.f));
}

TEST_CASE("Scaling a ray")
{
    Ray R(Point(1.f, 2.f, 3.f), Vector(0.f, 1.f, 0.f));
    auto M = Matrix::Scaling(2.f, 3.f, 4.f);
    auto R2 = R.Transform(M);

    CHECK(R2.GetOrigin() == Point(2.f, 6.f, 12.f));
    CHECK(R2.GetDirection() == Vector(0.f, 3.f, 0.f));
}