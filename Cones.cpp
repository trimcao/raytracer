#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include <limits>
#include <algorithm>
#include "include/Cones.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"

Cones::Cones(int ID)
{
    Transform = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;
    Min = -std::numeric_limits<double>::infinity();
    Max = std::numeric_limits<double>::infinity();
    Closed = false;
}

Cones::Cones() : Cones(0)
{
}

int Cones::GetID()
{
    return ID;
}

Vector Cones::LocalNormalAt(Point &LocalPoint)
{   
    // compute the square of the distance from the y axis
    auto Dist = LocalPoint.X() * LocalPoint.X() + LocalPoint.Z() * LocalPoint.Z();

    if (Dist < (std::abs(Max)*std::abs(Max)) && (LocalPoint.Y() >= Max - Util::EPSILON))
        return Vector(0., 1., 0.);

    if (Dist < (std::abs(Min)*std::abs(Min)) && (LocalPoint.Y() <= Min + Util::EPSILON))
        return Vector(0., -1., 0.);

    auto Y = std::sqrt(Dist);
    if (LocalPoint.Y() > 0.)
        Y = -Y;
    return Vector(LocalPoint.X(), Y, LocalPoint.Z());
}

Vector Cones::LocalNormalAt(Point &&LocalPoint)
{
    return LocalNormalAt(LocalPoint);
}

std::vector<Intersection<Object>> Cones::LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr)
{
    std::vector<Intersection<Object>> Intersections;

    auto RayDirection = LocalRay.GetDirection();
    auto RayOrigin = LocalRay.GetOrigin();

    auto A = RayDirection.X() * RayDirection.X() - RayDirection.Y() * RayDirection.Y() +
                RayDirection.Z() * RayDirection.Z();
    auto B = 2. * RayOrigin.X() * RayDirection.X() - 2. * RayOrigin.Y() * RayDirection.Y() +
                2. * RayOrigin.Z() * RayDirection.Z();
    auto C = RayOrigin.X() * RayOrigin.X() - RayOrigin.Y() * RayOrigin.Y() +
                RayOrigin.Z() * RayOrigin.Z();

    // check case: if A == 0 then ray is parallel to one of the halves
    if (!Util::Equal(A, 0.))
    {
        auto Disc = B * B - 4 * A * C;
        // check case: ray does not intersect the Cone
        if (Disc < 0)
            return Intersections;

        auto T0 = (-B - std::sqrt(Disc)) / (2 * A);
        auto T1 = (-B + std::sqrt(Disc)) / (2 * A);
        if (T0 > T1) std::swap(T0, T1);

        auto Y0 = RayOrigin.Y() + T0 * RayDirection.Y();
        if (Min < Y0 && Y0 < Max)
            Intersections.push_back(Intersection(T0, ObjectPtr));

        auto Y1 = RayOrigin.Y() + T1 * RayDirection.Y();
        if (Min < Y1 && Y1 < Max)
            Intersections.push_back(Intersection(T1, ObjectPtr));
    }
    else
    {
        auto T = (-C) / (2 * B);
        Intersections.push_back(Intersection(T, ObjectPtr));
    }

    // check intersection with caps
    IntersectCaps(LocalRay, Intersections, ObjectPtr);

    return Intersections;
}

bool Cones::CheckCap(const Ray &R, double T, double AbsY)
{
    auto X = R.GetOrigin().X() + T * R.GetDirection().X();
    auto Z = R.GetOrigin().Z() + T * R.GetDirection().Z();

    return (X * X + Z * Z) <= (AbsY * AbsY);
}

void Cones::IntersectCaps(const Ray &R, std::vector<Intersection<Object>> &Intersections, std::shared_ptr<Object> &ObjectPtr)
{
    // Caps only matter if the Cone is closed, and might
    // possibly be intersected by the ray
    if (!Closed || Util::Equal(R.GetDirection().Y(), 0.))
        return;

    double T;
    // check for an intersection with the lower end cap by intersecting
    // the ray with the plane at y = GetMin()
    T = (Min - R.GetOrigin().Y()) / R.GetDirection().Y();
    if (CheckCap(R, T, std::abs(Min)))
    {
        Intersections.push_back(Intersection(T, ObjectPtr));
    }

    // same as above for GetMax()
    T = (Max - R.GetOrigin().Y()) / R.GetDirection().Y();
    if (CheckCap(R, T, std::abs(Max)))
    {
        Intersections.push_back(Intersection(T, ObjectPtr));
    }
}

TEST_CASE("Intersecting a cone with a ray")
{
    std::shared_ptr<Object> Cone = std::make_shared<Cones>(Cones());
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cone->LocalIntersect(R, Cone);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 5.));
    CHECK(Util::Equal(XS[1].GetT(), 5.));

    Direction = Vector(1., 1., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cone->LocalIntersect(R, Cone);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 8.66025));
    CHECK(Util::Equal(XS[1].GetT(), 8.66025));

    Direction = Vector(-0.5, -1., 1.).Normalize();
    R = Ray(Point(1., 1., -5.), Direction);
    XS = Cone->LocalIntersect(R, Cone);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.55006));
    CHECK(Util::Equal(XS[1].GetT(), 49.44994));
}

TEST_CASE("Intersecting a cone with a ray parallel to one of its halves")
{
    std::shared_ptr<Object> Cone = std::make_shared<Cones>(Cones());
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 1., 1.).Normalize();
    R = Ray(Point(0., 0., -1.), Direction);
    XS = Cone->LocalIntersect(R, Cone);
    CHECK(XS.size() == 1);
    CHECK(Util::Equal(XS[0].GetT(), 0.35355));
}

TEST_CASE("Intersecting a cone's end caps")
{
    Cones Cone;
    Cone.SetMin(-0.5);
    Cone.SetMax(0.5);
    Cone.SetClosed(true);

    std::shared_ptr<Object> ConePtr = std::make_shared<Cones>(Cone);
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = ConePtr->LocalIntersect(R, ConePtr);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 1., 1.).Normalize();
    R = Ray(Point(0., 0., -0.25), Direction);
    XS = ConePtr->LocalIntersect(R, ConePtr);
    CHECK(XS.size() == 2);

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(0., 0., -0.25), Direction);
    XS = ConePtr->LocalIntersect(R, ConePtr);
    CHECK(XS.size() == 4);
}

TEST_CASE("Computing the normal vector on a cone")
{
    std::shared_ptr<Object> Cone = std::make_shared<Cones>(Cones());
    Vector N;

    N = Cone->LocalNormalAt(Point(0., 0., 0.));
    CHECK(N == Vector(0., 0., 0.));

    N = Cone->LocalNormalAt(Point(1., 1., 1.));
    CHECK(N == Vector(1., -std::sqrt(2.), 1.));

    N = Cone->LocalNormalAt(Point(-1., -1., 0.));
    CHECK(N == Vector(-1., 1., 0.));
}