#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/Cubes.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"

Cubes::Cubes(int ID)
{
    Transform = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Parent = nullptr;
}

Cubes::Cubes() : Cubes(0)
{
}

int Cubes::GetID()
{
    return ID;
}

Vector Cubes::LocalNormalAt(Point &LocalPoint)
{
    auto MaxC = std::max(std::abs(LocalPoint.X()), std::abs(LocalPoint.Y()));
    MaxC = std::max(MaxC, std::abs(LocalPoint.Z()));

    if (MaxC == std::abs(LocalPoint.X()))
        return Vector(LocalPoint.X(), 0., 0.);
    else if (MaxC == std::abs(LocalPoint.Y()))
        return Vector(0., LocalPoint.Y(), 0.);
    
    return Vector(0., 0., LocalPoint.Z());
}

Vector Cubes::LocalNormalAt(Point &&LocalPoint)
{
    return LocalNormalAt(LocalPoint);
}

std::vector<Intersection<Object>> Cubes::LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr)
{
    std::vector<Intersection<Object>> Intersections;

    auto XT = CheckAxis(LocalRay.GetOrigin().X(), LocalRay.GetDirection().X());
    auto YT = CheckAxis(LocalRay.GetOrigin().Y(), LocalRay.GetDirection().Y());
    auto ZT = CheckAxis(LocalRay.GetOrigin().Z(), LocalRay.GetDirection().Z());

    std::vector<double> TMinCandidates {XT[0], YT[0], ZT[0]};
    std::vector<double> TMaxCandidates {XT[1], YT[1], ZT[1]};
    auto TMin = *std::max_element(TMinCandidates.begin(), TMinCandidates.end());
    auto TMax = *std::min_element(TMaxCandidates.begin(), TMaxCandidates.end());

    if (TMin > TMax)
        return Intersections;

    Intersections.push_back(Intersection(TMin, ObjectPtr));
    Intersections.push_back(Intersection(TMax, ObjectPtr));

    return Intersections;
}

std::vector<double> CheckAxis(double Origin, double Direction)
{
    // we consider two planes with offset -1 and 1 from origin,
    // respectively.
    auto TMinNumerator = (-1 - Origin);
    auto TMaxNumerator = (1 - Origin);
    double TMin, TMax;

    if (std::abs(Direction) >= Util::EPSILON)
    {
        TMin = TMinNumerator / Direction;
        TMax = TMaxNumerator / Direction;
    }
    else
    {
        auto inf = std::numeric_limits<double>::infinity();
        TMin = TMinNumerator * inf;
        TMax = TMaxNumerator * inf;
    }

    if (TMin > TMax)
        std::swap(TMin, TMax);

    return std::vector<double> { TMin, TMax };
}

TEST_CASE("A ray intersects a cube")
{
    std::shared_ptr<Object> C = std::make_shared<Cubes>(Cubes());
    Ray R;
    std::vector<Intersection<Object>> XS;

    R = Ray(Point(5., 0.5, 0.), Vector(-1., 0., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(-5., 0.5, 0.), Vector(1., 0., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(0.5, 5., 0.), Vector(0., -1., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(0.5, -5., 0.), Vector(0., 1., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(0.5, 0., 5.), Vector(0., 0., -1.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(0.5, 0., -5.), Vector(0., 0., 1.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 4.);
    CHECK(XS[1].GetT() == 6.);

    R = Ray(Point(0., 0.5, 0.), Vector(0., 0., 1.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == -1.);
    CHECK(XS[1].GetT() == 1.);
}

TEST_CASE("A ray misses a cube")
{
    std::shared_ptr<Object> C = std::make_shared<Cubes>(Cubes());
    Ray R;
    std::vector<Intersection<Object>> XS;

    R = Ray(Point(-2., 0., 0.), Vector(0.2673, 0.5345, 0.8018));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);

    R = Ray(Point(0., -2., 0.), Vector(0.8018, 0.2673, 0.5345));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);

    R = Ray(Point(0., 0., -2.), Vector(0.5345, 0.8018, 0.2673));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);

    R = Ray(Point(2., 0., 2.), Vector(0., 0., -1.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);

    R = Ray(Point(0., 2., 2.), Vector(0., -1., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);

    R = Ray(Point(2., 2., 0.), Vector(-1., 0., 0.));
    XS = C->LocalIntersect(R, C);
    CHECK(XS.size() == 0);
}

TEST_CASE("The normal on the surface of a cube")
{
    std::shared_ptr<Object> C = std::make_shared<Cubes>(Cubes());
    Point P;
    Vector Normal;

    P = Point(1., 0.5, -0.8);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(1., 0., 0.));

    P = Point(-1., -0.2, 0.9);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(-1., 0., 0.));

    P = Point(-0.4, 1., -0.1);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(0., 1., 0.));

    P = Point(0.3, -1., -0.7);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(0., -1., 0.));

    P = Point(-0.6, 0.3, 1.);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(0., 0., 1.));
    
    P = Point(0.4, 0.4, -1.);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(0., 0., -1.));

    P = Point(1., 1., 1.);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(1., 0., 0.));

    P = Point(-1., -1., -1.);
    Normal = C->LocalNormalAt(P);
    CHECK(Normal == Vector(-1., 0., 0.));
}