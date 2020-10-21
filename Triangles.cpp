#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/Triangles.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"

Triangles::Triangles(Point &&Point1, Point &&Point2, Point &&Point3) : Triangles()
{
    P1 = Point1;
    P2 = Point2;
    P3 = Point3;
    E1 = P2 - P1;
    E2 = P3 - P1;
    Normal = E2.Cross(E1).Normalize();
}

Triangles::Triangles(Point &Point1, Point &Point2, Point &Point3) : Triangles()
{
    P1 = Point1;
    P2 = Point2;
    P3 = Point3;
    E1 = P2 - P1;
    E2 = P3 - P1;
    Normal = E2.Cross(E1).Normalize();
}

Triangles::Triangles(int ID)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Parent = nullptr;
}

Triangles::Triangles() : Triangles(0)
{
}

int Triangles::GetID()
{
    return ID;
}

Vector Triangles::LocalNormalAt(Point &LocalPoint)
{    
    return Normal;
}

Vector Triangles::LocalNormalAt(Point &&LocalPoint)
{
    return LocalNormalAt(LocalPoint);
}

std::vector<Intersection<Object>> Triangles::LocalIntersect(const Ray &LocalRay)
{
    std::vector<Intersection<Object>> Intersections;

    auto DirCrossE2 = LocalRay.GetDirection().Cross(E2);
    auto Determinant = E1.Dot(DirCrossE2);

    if (std::abs(Determinant) < Util::EPSILON)
        return Intersections;
    
    auto F = 1. / Determinant;
    auto P1ToOrigin = LocalRay.GetOrigin() - P1;
    auto U = F * P1ToOrigin.Dot(DirCrossE2);

    if (U < 0. || U > 1.)
        return Intersections;

    auto OriginCrossE1 = P1ToOrigin.Cross(E1);
    auto V = F * LocalRay.GetDirection().Dot(OriginCrossE1);

    if (V < 0. || (U + V) > 1.)
        return Intersections;

    auto T = F * E2.Dot(OriginCrossE1);
    Intersections.push_back(Intersection<Object>(T, this));
    return Intersections;
}

TEST_CASE("Constructing a triangle")
{
    Point P1(0., 1., 0.);
    Point P2(-1., 0., 0.);
    Point P3(1., 0., 0.);
    Triangles T(P1, P2, P3);

    CHECK(T.GetP1() == P1);
    CHECK(T.GetP2() == P2);
    CHECK(T.GetP3() == P3);
    CHECK(T.GetE1() == Vector(-1., -1., 0.));
    CHECK(T.GetE2() == Vector(1., -1., 0.));
    CHECK(T.GetNormal() == Vector(0., 0., -1.));
}

TEST_CASE("Finding the normal on a triangle")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    auto N1 = T.LocalNormalAt(Point(0., 0.5, 0.));
    auto N2 = T.LocalNormalAt(Point(-0.5, 0.75, 0.));
    auto N3 = T.LocalNormalAt(Point(0.5, 0.25, 0.));

    CHECK(N1 == T.GetNormal());
    CHECK(N2 == T.GetNormal());
    CHECK(N3 == T.GetNormal());
}

TEST_CASE("Intersecting a ray parallel to the triangle")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Ray R(Point(0., -1., -2.), Vector(0., 1., 0.));

    auto XS = T.LocalIntersect(R);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray misses the P1-P3 edge")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Ray R(Point(1., 1., -2.), Vector(0., 0., 1.));
    auto XS = T.LocalIntersect(R);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray misses the P1-P2 edge")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Ray R(Point(-1., 1., -2.), Vector(0., 0., 1.));
    auto XS = T.LocalIntersect(R);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray misses the P2-P3 edge")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Ray R(Point(0., -1., -2.), Vector(0., 0., 1.));
    auto XS = T.LocalIntersect(R);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray strikes a triangle")
{
    Triangles T(Point(0., 1, 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Ray R(Point(0., 0.5, -2.), Vector(0., 0., 1.));
    auto XS = T.LocalIntersect(R);
    CHECK(XS.size() == 1);
    CHECK(Util::Equal(XS[0].GetT(), 2.));
}