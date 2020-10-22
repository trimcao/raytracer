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
#include "include/Functions.h"

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

SmoothTriangles::SmoothTriangles(Point &&P1, Point &&P2, Point &&P3, Vector &&N1, Vector &&N2, Vector &&N3) : SmoothTriangles()
{
    this->P1 = P1;
    this->P2 = P2;
    this->P3 = P3;
    E1 = P2 - P1;
    E2 = P3 - P1;
    this->N1 = N1;
    this->N2 = N2;
    this->N3 = N3;
}

SmoothTriangles::SmoothTriangles(Point &P1, Point &P2, Point &P3, Vector &N1, Vector &N2, Vector &N3) : SmoothTriangles()
{
    this->P1 = P1;
    this->P2 = P2;
    this->P3 = P3;
    E1 = P2 - P1;
    E2 = P3 - P1;
    this->N1 = N1;
    this->N2 = N2;
    this->N3 = N3;
}

SmoothTriangles::SmoothTriangles(int ID)
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Parent = nullptr;
}

SmoothTriangles::SmoothTriangles() : SmoothTriangles(0)
{
}

Vector SmoothTriangles::LocalNormalAt(Point &LocalPoint, Intersection<Object> &I)
{
    return N2 * I.GetU() +
            N3 * I.GetV() +
            N1 * (1 - I.GetU() - I.GetV());
}

Vector SmoothTriangles::LocalNormalAt(Point &&LocalPoint, Intersection<Object> &I)
{
    return LocalNormalAt(LocalPoint, I);
}

std::vector<Intersection<Object>> SmoothTriangles::LocalIntersect(const Ray &LocalRay)
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
    Intersections.push_back(Intersection<Object>(T, this, U, V));
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

TEST_CASE("Constructing a smooth triangle")
{
    Point P1(0., 1., 0.);
    Point P2(-1., 0., 0.);
    Point P3(1., 0., 0.);
    Vector N1(0., 1., 0.);
    Vector N2(-1., 0., 0.);
    Vector N3(1., 0., 0.);

    SmoothTriangles Tri(P1, P2, P3, N1, N2, N3);
    CHECK(Tri.GetP1() == P1);
    CHECK(Tri.GetP2() == P2);
    CHECK(Tri.GetP3() == P3);
    CHECK(Tri.GetN1() == N1);
    CHECK(Tri.GetN2() == N2);
    CHECK(Tri.GetN3() == N3);
}

TEST_CASE("An intersection can encapsulate 'u' and 'v'")
{
    Triangles S(Point(0., 1., 0.), Point(-1., 0., 0.), Point(1., 0., 0.));
    Intersection I(3.5, &S, 0.2, 0.4);
    CHECK(I.GetU() == 0.2);
    CHECK(I.GetV() == 0.4);
}

TEST_CASE("Constructing a smooth triangle")
{
    Point P1(0., 1., 0.);
    Point P2(-1., 0., 0.);
    Point P3(1., 0., 0.);
    Vector N1(0., 1., 0.);
    Vector N2(-1., 0., 0.);
    Vector N3(1., 0., 0.);

    SmoothTriangles Tri(P1, P2, P3, N1, N2, N3);

    Ray R(Point(-0.2, 0.3, -2.), Vector(0., 0., 1.));
    auto XS = Tri.LocalIntersect(R);

    CHECK(Util::Equal(XS[0].GetU(), 0.45));
    CHECK(Util::Equal(XS[0].GetV(), 0.25));
}

TEST_CASE("A smooth triangle uses u/v to interpolate the normal")
{
    Point P1(0., 1., 0.);
    Point P2(-1., 0., 0.);
    Point P3(1., 0., 0.);
    Vector N1(0., 1., 0.);
    Vector N2(-1., 0., 0.);
    Vector N3(1., 0., 0.);
    SmoothTriangles Tri(P1, P2, P3, N1, N2, N3);

    Intersection<Object> I(1., &Tri, 0.45, 0.25);
    auto N = Tri.NormalAt(Point(0., 0., 0.), I);
    CHECK(N == Vector(-0.5547, 0.83205, 0.));
}

TEST_CASE("Preparing the normal on a smooth triangle")
{
    Point P1(0., 1., 0.);
    Point P2(-1., 0., 0.);
    Point P3(1., 0., 0.);
    Vector N1(0., 1., 0.);
    Vector N2(-1., 0., 0.);
    Vector N3(1., 0., 0.);
    SmoothTriangles Tri(P1, P2, P3, N1, N2, N3);

    Ray R(Point(-0.2, 0.3, -2.), Vector(0., 0., 1.));
    Intersection<Object> I(1., &Tri, 0.45, 0.25);
    std::vector<Intersection<Object>> XS {I};
    auto Comps = TRay::PrepareComputations(I, R, XS);

    CHECK(Comps.NormalV == Vector(-0.5547, 0.83205, 0.));
}