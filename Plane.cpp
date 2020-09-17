#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include "include/Plane.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Util.h"

Plane::Plane(int ID)
{
    Transform = Matrix::Identity();
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    this->ID = ID;
}

Plane::Plane() : Plane(0)
{
}

int Plane::GetID()
{
    return ID;
}

Vector Plane::LocalNormalAt(Point &LocalPoint)
{
    return Vector(0., 1., 0.);
}

Vector Plane::LocalNormalAt(Point &&LocalPoint)
{
    return Vector(0., 1., 0.);
}

std::vector<Intersection<Object>> Plane::LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr)
{
    std::vector<Intersection<Object>> Intersections;

    if (std::abs(LocalRay.GetDirection().Y()) < Util::EPSILON)
    {
        return Intersections;
    }

    auto T = -LocalRay.GetOrigin().Y() / LocalRay.GetDirection().Y();
    Intersections.push_back(Intersection(T, ObjectPtr));

    return Intersections;
}

TEST_CASE("The normal of a plane is constant everywhere")
{
    Plane P;
    auto N1 = P.LocalNormalAt(Point(0., 0., 0.));
    auto N2 = P.LocalNormalAt(Point(10., 0., -10.));
    auto N3 = P.LocalNormalAt(Point(-5., 0., 150.));

    CHECK(N1 == Vector(0., 1., 0.));
    CHECK(N2 == Vector(0., 1., 0.));
    CHECK(N3 == Vector(0., 1., 0.));
}

TEST_CASE("Intersect with a ray parallel to the plane")
{
    Plane P;
    std::shared_ptr<Object> Ptr = std::make_shared<Plane>(P);
    Ray R(Point(0., 10., 0.), Vector(0., 0., 1.));
    auto XS = P.LocalIntersect(R, Ptr);

    CHECK(XS.size() == 0);
}

TEST_CASE("Intersect with a coplanar ray")
{
    Plane P;
    std::shared_ptr<Object> Ptr = std::make_shared<Plane>(P);
    Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
    auto XS = P.LocalIntersect(R, Ptr);

    CHECK(XS.size() == 0);
}

TEST_CASE("A ray intersecting a plane from above")
{
    Plane P;
    std::shared_ptr<Object> Ptr = std::make_shared<Plane>(P);
    Ray R(Point(0., 1., 0.), Vector(0., -1., 0.));
    auto XS = P.LocalIntersect(R, Ptr);

    CHECK(XS.size() == 1);
    CHECK(Util::Equal(XS[0].GetT(), 1.f));
    CHECK(XS[0].GetObject() == Ptr);
}

TEST_CASE("A ray intersecting a plane from below")
{
    Plane P;
    std::shared_ptr<Object> Ptr = std::make_shared<Plane>(P);
    Ray R(Point(0., -1., 0.), Vector(0., 1., 0.));
    auto XS = P.LocalIntersect(R, Ptr);

    CHECK(XS.size() == 1);
    CHECK(Util::Equal(XS[0].GetT(), 1.f));
    CHECK(XS[0].GetObject() == Ptr);
}