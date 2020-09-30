#include <iostream>
#include <exception>
#include <stdexcept>
#include <cmath>
#include <memory>
#include "include/Sphere.h"
#include "include/Ray.h"
#include "include/Intersection.h"
#include "include/Transformations.h"
#include "include/Pattern.h"
#include "include/Functions.h"

Sphere::Sphere(int ID)
{
    Transform = Matrix::Identity(4);
    Origin = Point(0., 0., 0.);
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

Vector Sphere::LocalNormalAt(Point &LocalPoint)
{
    return LocalPoint - Origin;
}

std::vector<Intersection<Object>> Sphere::Intersect(const Ray &R)
{
    std::vector<Intersection<Object>> Intersections;

    auto TransformedRay = R.Transform(Transform.Inverse());

    // assume the origin of Sphere is always (0., 0., 0.)
    Vector SphereToRay = TransformedRay.GetOrigin() - Point(0., 0., 0.);
    double A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
    double B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
    double C = SphereToRay.Dot(SphereToRay) - 1.;

    double Discriminant = B * B - 4 * A * C;

    if (Discriminant >= 0.)
    {
        Intersections.push_back(Intersection<Object>((-B - std::sqrt(Discriminant)) / (2 * A), *this));
        Intersections.push_back(Intersection<Object>((-B + std::sqrt(Discriminant)) / (2 * A), *this));
    }

    return Intersections;
}

std::vector<Intersection<Object>> Sphere::LocalIntersect(const Ray &LocalRay, std::shared_ptr<Object> &ObjectPtr)
{
    std::vector<Intersection<Object>> Intersections;

    // assume the origin of Sphere is always (0., 0., 0.)
    Vector SphereToRay = LocalRay.GetOrigin() - Point(0., 0., 0.);
    double A = LocalRay.GetDirection().Dot(LocalRay.GetDirection());
    double B = 2 * LocalRay.GetDirection().Dot(SphereToRay);
    double C = SphereToRay.Dot(SphereToRay) - 1.;

    double Discriminant = B * B - 4 * A * C;

    if (Discriminant >= 0.)
    {
        Intersections.push_back(Intersection<Object>((-B - std::sqrt(Discriminant)) / (2 * A), ObjectPtr));
        Intersections.push_back(Intersection<Object>((-B + std::sqrt(Discriminant)) / (2 * A), ObjectPtr));
    }

    return Intersections;
}

Sphere Sphere::GlassSphere()
{
    Sphere S;
    Material Mat;
    Mat.SetTransparency(1.);
    Mat.SetRefractiveIndex(1.5);
    S.SetMaterial(Mat);
    return S;
}

TEST_CASE("A sphere's default transformation")
{
    Sphere S(2);
    CHECK(S.GetTransform() == Matrix::Identity(4));
}

TEST_CASE("Changing a sphere's transformation")
{
    Sphere S(2);
    auto M = Matrix::Translation(2., 3., 4.);
    S.SetTransform(M);

    CHECK(S.GetTransform() == Matrix::Translation(2., 3., 4.));
}

TEST_CASE("Intersecting a scaled sphere with a ray")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere S(2);
    S.SetTransform(Matrix::Scaling(2., 2., 2.));

    auto XS = S.Intersect(R);

    CHECK(XS.size() == 2);
    CHECK(XS[0].GetT() == 3.);
    CHECK(XS[1].GetT() == 7.);
}

TEST_CASE("Intersecting a translated sphere with a ray")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere S(2);
    S.SetTransform(Matrix::Translation(5., 0., 0.));

    auto XS = S.Intersect(R);

    CHECK(XS.size() == 0);
}

TEST_CASE("The normal on a sphere at a point on the x axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(1., 0., 0.));
    CHECK(N == Vector(1., 0., 0.));
}

TEST_CASE("The normal on a sphere at a point on the y axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(0., 1., 0.));
    CHECK(N == Vector(0., 1., 0.));
}

TEST_CASE("The normal on a sphere at a point on the z axis")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(0., 0., 1.));
    CHECK(N == Vector(0., 0., 1.));
}

TEST_CASE("The normal on a sphere at a nonaxial point")
{
    Sphere S(1);
    auto N = S.NormalAt(Point(std::sqrt(3.) / 3, std::sqrt(3.) / 3, std::sqrt(3.) / 3));
    CHECK(N == Vector(std::sqrt(3.) / 3, std::sqrt(3.) / 3, std::sqrt(3.) / 3));
}

TEST_CASE("Computing the normal on a translated sphere")
{
    Sphere S(1);
    S.SetTransform(Matrix::Translation(0., 1., 0.));
    auto N = S.NormalAt(Point(0., 1.70711, -0.70711));
    CHECK(N == Vector(0., 0.70711, -0.70711));
}

TEST_CASE("Computing the normal on a transformed sphere")
{
    Sphere S(1);
    Matrix M = Matrix::Scaling(1., 0.5, 1.0).Mul(Matrix::RotationZ(M_PI / 5));
    S.SetTransform(M);
    auto N = S.NormalAt(Point(0., std::sqrt(2.) / 2, -std::sqrt(2.) / 2));
    CHECK(N == Vector(0., 0.97014, -0.24254));
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
    M.SetAmbient(1.);
    S.SetMaterial(M);
    Material N;
    N.SetAmbient(1.);

    CHECK(S.GetMaterial() == N);
}

TEST_CASE("A ray intersects a sphere at two points")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere S;

    auto XS = S.Intersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.0));
    CHECK(Util::Equal(XS[1].GetT(), 6.0));
}

TEST_CASE("A ray misses a sphere")
{
    Ray R(Point(0., 2., -5.), Vector(0., 0., 1.));
    Sphere S;

    auto XS = S.Intersect(R);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray originates inside a sphere")
{
    Ray R(Point(0., 0., 0.), Vector(0., 0., 1.));
    Sphere S;

    auto XS = S.Intersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -1.0));
    CHECK(Util::Equal(XS[1].GetT(), 1.0));
}

TEST_CASE("A sphere is behind a ray")
{
    Ray R(Point(0., 0., 5.), Vector(0., 0., 1.));
    Sphere S;

    auto XS = S.Intersect(R);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -6.0));
    CHECK(Util::Equal(XS[1].GetT(), -4.0));
}

TEST_CASE("An intersection encapsulates t and object")
{
    Sphere S(3);
    Intersection I(3.5, S);

    CHECK(Util::Equal(I.GetT(), 3.5));
    CHECK(I.GetObject()->GetID() == 3);
}

TEST_CASE("The hit, when all intersections have positive t")
{
    Sphere S(1);
    Intersection I1(1., S);
    Intersection I2(2., S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(*Hit(XS) == I1);
}

TEST_CASE("The hit, when some intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-1., S);
    Intersection I2(1., S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(*Hit(XS) == I2);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-2., S);
    Intersection I2(-1., S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(Hit(XS) == nullptr);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(5., S);
    Intersection I2(7., S);
    Intersection I3(-3., S);
    Intersection I4(2., S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2, I3, I4});

    CHECK(*Hit(XS) == I4);
}

TEST_CASE("The hit should offset the point")
{
    Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
    Sphere Shape;
    Shape.SetTransform(Matrix::Translation(0., 0., 1.));
    Intersection I(5., Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.OverPosition.Z() < -Util::EPSILON/2);
    CHECK(Comps.Position.Z() > Comps.OverPosition.Z());
}
