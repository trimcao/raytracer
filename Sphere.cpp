#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include "include/Sphere.h"
#include "include/Ray.h"
#include "include/Intersection.h"

Sphere::Sphere(int ID)
{
    Transform = Matrix::Identity(4);
    Origin = Point(0., 0., 0.f);
    AMaterial = Material();
    this->ID = ID;
}

Sphere::Sphere() : Sphere(0)
{
}

int Sphere::GetID()
{
    return ID;
}

Vector Sphere::NormalAt(Point &P)
{
    auto ObjectPoint = Transform.Inverse().Mul(P);
    auto ObjectNormal = ObjectPoint - Origin;
    auto WorldNormal = Transform.Inverse().T().Mul(ObjectNormal);
    WorldNormal.SetW(0.f);
    return WorldNormal.Normalize();
}

Vector Sphere::NormalAt(Point &&P)
{
    return NormalAt(P);
}

TEST_CASE("A sphere's default transformation")
{
    Sphere S(2);
    CHECK(S.GetTransform() == Matrix::Identity(4));
}

TEST_CASE("Changing a sphere's transformation")
{
    Sphere S(2);
    auto M = Matrix::Translation(2., 3., 4.f);
    S.SetTransform(M);

    CHECK(S.GetTransform() == Matrix::Translation(2., 3., 4.f));
}

TEST_CASE("Intersecting a scaled sphere with a ray")
{
    Ray R(Point(0., 0., -5.f), Vector(0., 0., 1.f));
    Sphere S(2);
    S.SetTransform(Matrix::Scaling(2., 2., 2.f));

    auto XS = Intersect(R, S);

    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 3.f);
    CHECK(XS[1].GetT() == 7.f);
}

TEST_CASE("Intersecting a translated sphere with a ray")
{
    Ray R(Point(0., 0., -5.f), Vector(0., 0., 1.f));
    Sphere S(2);
    S.SetTransform(Matrix::Translation(5., 0., 0.f));

    auto XS = Intersect(R, S);

    CHECK(XS.size() == 0);
}

TEST_CASE("The normal on a sphere at a point on the x axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(1., 0., 0.f));
    CHECK(N == Vector(1., 0., 0.f));
}

TEST_CASE("The normal on a sphere at a point on the y axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(0., 1., 0.f));
    CHECK(N == Vector(0., 1., 0.f));
}

TEST_CASE("The normal on a sphere at a point on the z axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(0., 0., 1.f));
    CHECK(N == Vector(0., 0., 1.f));
}

TEST_CASE("The normal on a sphere at a nonaxial point")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(std::sqrt(3.f) / 3, std::sqrt(3.f) / 3, std::sqrt(3.f) / 3));
    CHECK(N == Vector(std::sqrt(3.f) / 3, std::sqrt(3.f) / 3, std::sqrt(3.f) / 3));
}

TEST_CASE("Computing the normal on a translated sphere")
{
    Sphere S(1);
    S.SetTransform(Matrix::Translation(0., 1., 0.f));
    auto N = S.NormalAt(Point(0., 1.70711, -0.70711f));
    CHECK(N == Vector(0., 0.70711, -0.70711f));
}

TEST_CASE("Computing the normal on a transformed sphere")
{
    Sphere S(1);
    Matrix M = Matrix::Scaling(1., 0.5, 1.0f).Mul(Matrix::RotationZ(M_PI / 5));
    S.SetTransform(M);
    auto N = S.NormalAt(Point(0., std::sqrt(2.f) / 2, -std::sqrt(2.f) / 2));
    CHECK(N == Vector(0., 0.97014, -0.24254f));
}

TEST_CASE("A sphere has a default material")
{
    Sphere S(1);
    Material M;

    CHECK(S.GetMaterial() == Material());
}

TEST_CASE("A sphere may be assigned a material")
{
    Sphere S(1);
    Material M;
    M.SetAmbient(1.f);
    S.SetMaterial(M);
    Material N;
    N.SetAmbient(1.f);

    CHECK(S.GetMaterial() == N);
}