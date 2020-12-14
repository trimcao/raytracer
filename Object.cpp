#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <cmath>
#include "include/Object.h"
#include "include/Transformations.h"
#include "include/Functions.h"

Object::Object()
{
    ID = 0;
}

int Object::GetID()
{
    return ID;
}

void Object::SetTransform(Matrix &M)
{
    Transform = M;
    TransformInverse = Transform.Inverse();
}

std::vector<Intersection<Object>> Object::Intersect(const Ray &R)
{
    auto LocalRay = R.Transform(TransformInverse);

    return LocalIntersect(LocalRay);
}

Vector Object::NormalAt(Point &P)
{
    auto LocalPoint = TRay::WorldToObject(this, P);
    auto LocalNormal = LocalNormalAt(LocalPoint);
    return TRay::NormalToWorld(this, LocalNormal);
}

Vector Object::NormalAt(Point &P, Intersection<Object> &I)
{
    auto LocalPoint = TRay::WorldToObject(this, P);
    auto LocalNormal = LocalNormalAt(LocalPoint, I);
    return TRay::NormalToWorld(this, LocalNormal);
}

TestShape::TestShape(int ID)
{
    Transform = Matrix::Identity(4);
    TransformInverse = Matrix::Identity(4);
    Origin = Point(0., 0., 0.);
    AMaterial = Material();
    UseShadow = true;
    this->ID = ID;

    Parent = nullptr;
}

TestShape::TestShape() : TestShape(0)
{
}

Vector TestShape::LocalNormalAt(Point &LocalPoint)
{
    return Vector(LocalPoint.X(), LocalPoint.Y(), LocalPoint.Z());
}

std::vector<Intersection<Object>> TestShape::LocalIntersect(const Ray &R)
{
    SavedRay = R;

    return std::vector<Intersection<Object>>();
}

// TEST_CASE("The default transformation")
// {
//     TestShape S;
//     CHECK(S.GetTransform() == Matrix::Identity());
// }

// TEST_CASE("Assigning a transformation")
// {
//     TestShape S;
//     S.SetTransform(Transformations::Translation(2., 3., 4.));
//     CHECK(S.GetTransform() == Transformations::Translation(2., 3., 4.));
// }

// TEST_CASE("The default material")
// {
//     TestShape S;
//     CHECK(S.GetMaterial() == Material());
// }

// TEST_CASE("Assigning a material")
// {
//     TestShape S;
//     Material M;
//     M.SetAmbient(1.);
//     S.SetMaterial(M);
//     CHECK(S.GetMaterial() == M);
// }

// TEST_CASE("Intersecting a scaled shape with a ray")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     TestShape S;
//     S.SetTransform(Transformations::Scaling(2., 2., 2.));
//     auto SPtr = std::make_shared<Object>(S);
//     auto XS = S.Intersect(R);

//     CHECK(S.SavedRay.GetOrigin() == Point(0., 0., -2.5));
//     CHECK(S.SavedRay.GetDirection() == Vector(0., 0., 0.5));
// }

// TEST_CASE("Intersecting a translated shape with a ray")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     TestShape S;
//     S.SetTransform(Transformations::Translation(5., 0., 0.));
//     auto SPtr = std::make_shared<Object>(S);
//     auto XS = S.Intersect(R);

//     CHECK(S.SavedRay.GetOrigin() == Point(-5., 0., -5.));
//     CHECK(S.SavedRay.GetDirection() == Vector(0., 0., 1.));
// }

// TEST_CASE("Computing the normal on a translated shape")
// {
//     TestShape S;
//     S.SetTransform(Transformations::Translation(0., 1., 0.));
//     auto N = S.NormalAt(Point(0., 1.70711, -0.70711));

//     CHECK(N == Vector(0., 0.70711, -0.70711));
// }

// TEST_CASE("Computing the normal on a transformed shape")
// {
//     TestShape S;
//     auto M = Transformations::Scaling(1.f, 0.5f, 1.f).Mul(Transformations::RotationZ(M_PI/5));
//     S.SetTransform(M);
//     auto N = S.NormalAt(Point(0., std::sqrt(2.f)/2, -std::sqrt(2.f)/2));

//     CHECK(N == Vector(0., 0.97014, -0.24254));
// }

// TEST_CASE("A shape has a parent attribute")
// {
//     TestShape S;
//     CHECK(S.GetParent() == nullptr);
// }