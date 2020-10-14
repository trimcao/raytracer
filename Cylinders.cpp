#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/Cylinders.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"

Cylinders::Cylinders(int ID)
{
    Transform = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Min = -std::numeric_limits<double>::infinity();
    Max = std::numeric_limits<double>::infinity();
    Closed = false;
    Parent = nullptr;
}

Cylinders::Cylinders() : Cylinders(0)
{
}

int Cylinders::GetID()
{
    return ID;
}

Vector Cylinders::LocalNormalAt(Point &LocalPoint)
{   
    // compute the square of the distance from the y axis
    auto Dist = LocalPoint.X() * LocalPoint.X() + LocalPoint.Z() * LocalPoint.Z();

    if (Dist < 1.0 && (LocalPoint.Y() >= Max - Util::EPSILON))
        return Vector(0., 1., 0.);

    if (Dist < 1.0 && (LocalPoint.Y() <= Min + Util::EPSILON))
        return Vector(0., -1., 0.);

    return Vector(LocalPoint.X(), 0., LocalPoint.Z());
}

Vector Cylinders::LocalNormalAt(Point &&LocalPoint)
{
    return LocalNormalAt(LocalPoint);
}

std::vector<Intersection<Object>> Cylinders::LocalIntersect(const Ray &LocalRay)
{
    std::vector<Intersection<Object>> Intersections;

    auto RayDirection = LocalRay.GetDirection();
    auto RayOrigin = LocalRay.GetOrigin();

    auto A = RayDirection.X() * RayDirection.X() + RayDirection.Z() * RayDirection.Z();

    // check case: ray is parallel to the y axis
    if (!Util::Equal(A, 0.))
    {
        auto B = 2. * RayOrigin.X() * RayDirection.X() + 2. * RayOrigin.Z() * RayDirection.Z();
        auto C = RayOrigin.X() * RayOrigin.X() + RayOrigin.Z() * RayOrigin.Z() - 1.;

        auto Disc = B * B - 4 * A * C;
        // check case: ray does not intersect the cylinder
        if (Disc < 0)
            return Intersections;

        auto T0 = (-B - std::sqrt(Disc)) / (2 * A);
        auto T1 = (-B + std::sqrt(Disc)) / (2 * A);
        if (T0 > T1) std::swap(T0, T1);

        auto Y0 = RayOrigin.Y() + T0 * RayDirection.Y();
        if (Min < Y0 && Y0 < Max)
            Intersections.push_back(Intersection<Object>(T0, this));

        auto Y1 = RayOrigin.Y() + T1 * RayDirection.Y();
        if (Min < Y1 && Y1 < Max)
            Intersections.push_back(Intersection<Object>(T1, this));
    }

    // check intersection with caps
    IntersectCaps(LocalRay, Intersections);

    return Intersections;
}

bool Cylinders::CheckCap(const Ray &R, double T)
{
    auto X = R.GetOrigin().X() + T * R.GetDirection().X();
    auto Z = R.GetOrigin().Z() + T * R.GetDirection().Z();

    return (X * X + Z * Z) <= 1.;
}

void Cylinders::IntersectCaps(const Ray &R, std::vector<Intersection<Object>> &Intersections)
{
    // Caps only matter if the cylinder is closed, and might
    // possibly be intersected by the ray
    if (!Closed || Util::Equal(R.GetDirection().Y(), 0.))
        return;

    double T;
    // check for an intersection with the lower end cap by intersecting
    // the ray with the plane at y = cylinder.GetMin()
    T = (Min - R.GetOrigin().Y()) / R.GetDirection().Y();
    if (CheckCap(R, T))
    {
        Intersections.push_back(Intersection<Object>(T, this));
    }

    // same as above for cylinder.GetMax()
    T = (Max - R.GetOrigin().Y()) / R.GetDirection().Y();
    if (CheckCap(R, T))
    {
        Intersections.push_back(Intersection<Object>(T, this));
    }
}

TEST_CASE("A ray misses a cylinder")
{
    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinders());
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(1., 0., 0.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(0., 0., 0.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(1., 1., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);    
}

TEST_CASE("A ray strikes a cylinder")
{
    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinders());
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(1., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 5.));
    CHECK(Util::Equal(XS[1].GetT(), 5.));

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.));
    CHECK(Util::Equal(XS[1].GetT(), 6.));

    Direction = Vector(0.1, 1., 1.).Normalize();
    R = Ray(Point(0.5, 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 6.80798));
    CHECK(Util::Equal(XS[1].GetT(), 7.08872));
}

TEST_CASE("Normal vector on a cylinder")
{
    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinders());
    Vector N;

    N = Cyl->LocalNormalAt(Point(1., 0., 0.));
    CHECK(N == Vector(1., 0., 0.));

    N = Cyl->LocalNormalAt(Point(0., 5., -1.));
    CHECK(N == Vector(0., 0., -1.));

    N = Cyl->LocalNormalAt(Point(0., -2., 1.));
    CHECK(N == Vector(0., 0., 1.));

    N = Cyl->LocalNormalAt(Point(-1., 1., 0.));
    CHECK(N == Vector(-1., 0., 0.));
}

TEST_CASE("The default minimum and maximum for a cylinder")
{
    Cylinders Cyl;
    CHECK(Cyl.GetMin() == -std::numeric_limits<double>::infinity());
    CHECK(Cyl.GetMax() == std::numeric_limits<double>::infinity());
}

TEST_CASE("Intersecting a constrained cylinder")
{
    Cylinders Cylinder;
    Cylinder.SetMin(1.);
    Cylinder.SetMax(2.);

    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinder);
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0.1, 1., 0.).Normalize();
    R = Ray(Point(0., 1.5, 0.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 3., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 2., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 1., -5.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 1.5, -2.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);
}

TEST_CASE("The default closed value for a cylinder")
{
    Cylinders Cyl;
    CHECK(Cyl.IsClosed() == false);
}

TEST_CASE("Intersecting the caps of a closed cylinder")
{
    Cylinders Cylinder;
    Cylinder.SetMin(1.);
    Cylinder.SetMax(2.);
    Cylinder.SetClosed(true);

    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinder);
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., -1., 0.).Normalize();
    R = Ray(Point(0., 3., 0.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);

    Direction = Vector(0., -1., 2.).Normalize();
    R = Ray(Point(0., 3., -2.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);

    Direction = Vector(0., -1., 1.).Normalize();
    R = Ray(Point(0., 4., -2.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);

    Direction = Vector(0., 1., 2.).Normalize();
    R = Ray(Point(0., 0., -2.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);

    Direction = Vector(0., 1., 1.).Normalize();
    R = Ray(Point(0., -1., -2.), Direction);
    XS = Cyl->LocalIntersect(R);
    CHECK(XS.size() == 2);
}

TEST_CASE("Normal vector on a cylinder's end caps")
{
    Cylinders Cylinder;
    Cylinder.SetMin(1.);
    Cylinder.SetMax(2.);
    Cylinder.SetClosed(true);
    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinder);
    Vector N;

    N = Cyl->LocalNormalAt(Point(0., 1., 0.));
    CHECK(N == Vector(0., -1., 0.));

    N = Cyl->LocalNormalAt(Point(0.5, 1., 0.));
    CHECK(N == Vector(0., -1., 0.));

    N = Cyl->LocalNormalAt(Point(0., 1., 0.5));
    CHECK(N == Vector(0., -1., 0.));

    N = Cyl->LocalNormalAt(Point(0., 2., 0.));
    CHECK(N == Vector(0., 1., 0.));

    N = Cyl->LocalNormalAt(Point(0.5, 2., 0.));
    CHECK(N == Vector(0., 1., 0.));

    N = Cyl->LocalNormalAt(Point(0., 2., 0.5));
    CHECK(N == Vector(0., 1., 0.));
}