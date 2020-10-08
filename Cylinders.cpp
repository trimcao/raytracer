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
    return Vector(LocalPoint.X(), 0., LocalPoint.Z());
}

Vector Cylinders::LocalNormalAt(Point &&LocalPoint)
{
    return LocalNormalAt(LocalPoint);
}

std::vector<Intersection<Object>> Cylinders::LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr)
{
    std::vector<Intersection<Object>> Intersections;

    auto RayDirection = LocalRay.GetDirection();
    auto RayOrigin = LocalRay.GetOrigin();

    auto A = RayDirection.X() * RayDirection.X() + RayDirection.Z() * RayDirection.Z();
    // check case: ray is parallel to the y axis
    if (Util::Equal(A, 0.))
        return Intersections;

    auto B = 2 * RayOrigin.X() * RayDirection.X() + 2 * RayOrigin.Z() * RayDirection.Z();
    auto C = RayOrigin.X() * RayOrigin.X() + RayOrigin.Z() * RayOrigin.Z() - 1.; 

    auto Disc = B * B - 4 * A * C;
    // check case: ray does not intersect the cylinder
    if (Disc < 0)
        return Intersections;

    auto T0 = (-B - std::sqrt(Disc)) / (2 * A);
    auto T1 = (-B + std::sqrt(Disc)) / (2 * A);
    Intersections.push_back(Intersection(T0, ObjectPtr));
    Intersections.push_back(Intersection(T1, ObjectPtr));

    return Intersections;
}

TEST_CASE("A ray misses a cylinder")
{
    std::shared_ptr<Object> Cyl = std::make_shared<Cylinders>(Cylinders());
    Vector Direction;
    Ray R;
    std::vector<Intersection<Object>> XS;

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(1., 0., 0.), Direction);
    XS = Cyl->LocalIntersect(R, Cyl);
    CHECK(XS.size() == 0);

    Direction = Vector(0., 1., 0.).Normalize();
    R = Ray(Point(0., 0., 0.), Direction);
    XS = Cyl->LocalIntersect(R, Cyl);
    CHECK(XS.size() == 0);

    Direction = Vector(1., 1., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R, Cyl);
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
    XS = Cyl->LocalIntersect(R, Cyl);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 5.));
    CHECK(Util::Equal(XS[1].GetT(), 5.));

    Direction = Vector(0., 0., 1.).Normalize();
    R = Ray(Point(0., 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R, Cyl);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.));
    CHECK(Util::Equal(XS[1].GetT(), 6.));

    Direction = Vector(0.1, 1., 1.).Normalize();
    R = Ray(Point(0.5, 0., -5.), Direction);
    XS = Cyl->LocalIntersect(R, Cyl);
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