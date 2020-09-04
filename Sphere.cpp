#include <iostream>
#include <exception>
#include <stdexcept>
#include "include/Sphere.h"
#include "include/Ray.h"

Sphere::Sphere() {
    Transform = Matrix::Identity(4);
}

Sphere::Sphere(int ID) : Sphere()
{
    this->ID = ID;
}

int Sphere::GetID()
{
    return ID;
}

TEST_CASE("A sphere's default transformation")
{
    Sphere S(2);
    CHECK(S.GetTransform() == Matrix::Identity(4));
}

TEST_CASE("Changing a sphere's transformation")
{
    Sphere S(2);
    auto M = Matrix::Translation(2.f, 3.f, 4.f);
    S.SetTransform(M);

    CHECK(S.GetTransform() == Matrix::Translation(2.f, 3.f, 4.f));
}

TEST_CASE("Intersecting a scaled sphere with a ray")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S(2);
    S.SetTransform(Matrix::Scaling(2.f, 2.f, 2.f));

    auto XS = Intersect(R, S);

    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 3.f);
    CHECK(XS[1].GetT() == 7.f);
}

TEST_CASE("Intersecting a translated sphere with a ray")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S(2);
    S.SetTransform(Matrix::Translation(5.f, 0.f, 0.f));

    auto XS = Intersect(R, S);

    CHECK(XS.size() == 0);
}
