#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include "include/Ray.h"
#include "include/Sphere.h"

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

std::vector<float> Intersect(const Ray &R, const Sphere &S)
{
    std::vector<float> Intersections;

    // assume the origin of Sphere is always (0.f, 0.f, 0.f)
    Vector SphereToRay = R.GetOrigin() - Point(0.f, 0.f, 0.f);
    float A = R.GetDirection().Dot(R.GetDirection());
    float B = 2 * R.GetDirection().Dot(SphereToRay);
    float C = SphereToRay.Dot(SphereToRay) - 1.f;

    float Discriminant = B * B - 4 * A * C;

    if (Discriminant >= 0.f)
    {
        Intersections.push_back((-B - std::sqrt(Discriminant)) / (2 * A));
        Intersections.push_back((-B + std::sqrt(Discriminant)) / (2 * A));
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
    CHECK(Util::Equal(XS[0], 4.0));
    CHECK(Util::Equal(XS[1], 6.0));
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
    CHECK(Util::Equal(XS[0], -1.0));
    CHECK(Util::Equal(XS[1], 1.0));
}

TEST_CASE("A sphere is behind a ray")
{
    Ray R(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0], -6.0));
    CHECK(Util::Equal(XS[1], -4.0));
}