#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>
#include "include/Intersection.h"
#include "include/Ray.h"
#include "include/Sphere.h"
#include "include/Util.h"
#include "include/Light.h"

template <class OT>
Intersection<OT>::Intersection() {}

template <class OT>
float Intersection<OT>::GetT() const
{
    return T;
}

template<class OT>
std::shared_ptr<OT> Intersection<OT>::GetObject() const
{
    return O;
}

template<class OT>
Intersection<OT>::Intersection(float T, const OT &O)
{
    this->T = T;
    this->O = std::make_shared<OT>(O);
}

template<class OT>
Intersection<OT>::Intersection(float T, std::shared_ptr<OT> &O)
{
    this->T = T;
    this->O = O;
}

template<class OT>
std::ostream &operator<<(std::ostream &os, const Intersection<OT> &I)
{
    os << "Intersection{ " << I.GetT() << ", ";

    if (I.GetObject())
        os << I.GetObject()->GetID() << " }";
    else
        os << " }";

    return os;
}

template<class OT>
std::vector<Intersection<OT>> Intersections(std::vector<Intersection<OT>> &&I)
{
    std::sort(I.begin(), I.end());
    return I;
}

template<class OT>
void Intersections(std::vector<Intersection<OT>> &I)
{
    std::sort(I.begin(), I.end());
}

template<class OT>
std::shared_ptr<Intersection<OT>> Hit(std::vector<Intersection<OT>> &Intersections)
{
    for (auto I : Intersections)
    {
        if (I.GetT() > 0.f)
            return std::make_shared<Intersection<OT>>(I);
    }
    return nullptr;
}

template<class OT>
PreComputations<OT> Intersection<OT>::PrepareComputations(Ray &R)
{
    PreComputations<OT> Comps;
    Comps.T = T;
    Comps.AObject = O;
    Comps.Position = R.Position(T);
    Comps.EyeV = -R.GetDirection();
    Comps.NormalV = Comps.AObject->NormalAt(Comps.Position);

    if (Comps.NormalV.Dot(Comps.EyeV) < 0.f)
    {
        Comps.IsInside = true;
        Comps.NormalV = -Comps.NormalV;
    }

    Comps.OverPosition = Comps.Position + Comps.NormalV * Util::EPSILON;

    return Comps;
}

std::vector<Intersection<Sphere>> Intersect(const Ray &R, const Sphere &S)
{
    std::vector<Intersection<Sphere>> Intersections;

    auto TransformedRay = R.Transform(S.GetTransform().Inverse());

    // assume the origin of Sphere is always (0.f, 0.f, 0.f)
    Vector SphereToRay = TransformedRay.GetOrigin() - Point(0.f, 0.f, 0.f);
    float A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
    float B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
    float C = SphereToRay.Dot(SphereToRay) - 1.f;

    float Discriminant = B * B - 4 * A * C;

    if (Discriminant >= 0.f)
    {
        Intersections.push_back(Intersection((-B - std::sqrt(Discriminant)) / (2 * A), S));
        Intersections.push_back(Intersection((-B + std::sqrt(Discriminant)) / (2 * A), S));
    }

    return Intersections;
}

std::vector<Intersection<Object>> Intersect(const Ray &R, const World &W)
{
    std::vector<Intersection<Object>> Intersections;

    // now all objects are sphere
    for (auto O : W.GetObjects())
    {
        auto TransformedRay = R.Transform(O->GetTransform().Inverse());

        // assume the origin of Sphere is always (0.f, 0.f, 0.f)
        Vector SphereToRay = TransformedRay.GetOrigin() - Point(0.f, 0.f, 0.f);
        float A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
        float B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
        float C = SphereToRay.Dot(SphereToRay) - 1.f;

        float Discriminant = B * B - 4 * A * C;

        if (Discriminant >= 0.f)
        {
            Intersections.push_back(Intersection((-B - std::sqrt(Discriminant)) / (2 * A), O));
            Intersections.push_back(Intersection((-B + std::sqrt(Discriminant)) / (2 * A), O));
        }
    }

    // sort the intersections
    std::sort(Intersections.begin(), Intersections.end());

    return Intersections;
}

Color ShadeHit(World &W, PreComputations<Object> &Comps)
{
    if (!W.GetLight())
        return Color(0.f, 0.f, 0.f);

    bool IsInShadow = W.IsShadowed(Comps.OverPosition);
    // std::cout << "IsInShadow: " << IsInShadow << '\n';
    return Lighting(Comps.AObject->GetMaterial(), *W.GetLight(), Comps.OverPosition, Comps.EyeV, Comps.NormalV, IsInShadow);
}

Color ColorAt(World &W, Ray &R)
{
    auto Intersects = Intersect(R, W);
    auto H = Hit(Intersects);
    if (H != nullptr)
    {
        auto Comps = H->PrepareComputations(R);
        return ShadeHit(W, Comps);
    }

    return Color(0.f, 0.f, 0.f);
}

TEST_CASE("An intersection encapsulates t and object")
{
    Sphere S(3);
    Intersection I(3.5f, S);

    CHECK(Util::Equal(I.GetT(), 3.5f));
    CHECK(I.GetObject()->GetID() == 3);
}

TEST_CASE("The hit, when all intersections have positive t")
{
    Sphere S(1);
    Intersection I1(1.f, S);
    Intersection I2(2.f, S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(*Hit(XS) == I1);
}

TEST_CASE("The hit, when some intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-1.f, S);
    Intersection I2(1.f, S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(*Hit(XS) == I2);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(-2.f, S);
    Intersection I2(-1.f, S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

    CHECK(Hit(XS) == nullptr);
}

TEST_CASE("The hit, when all intersections have negative t")
{
    Sphere S(1);
    Intersection I1(5.f, S);
    Intersection I2(7.f, S);
    Intersection I3(-3.f, S);
    Intersection I4(2.f, S);
    auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2, I3, I4});

    CHECK(*Hit(XS) == I4);
}

TEST_CASE("A ray intersects a sphere at two points")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), 4.0));
    CHECK(Util::Equal(XS[1].GetT(), 6.0));
}

TEST_CASE("A ray misses a sphere")
{
    Ray R(Point(0.f, 2.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 0);
}

TEST_CASE("A ray originates inside a sphere")
{
    Ray R(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -1.0));
    CHECK(Util::Equal(XS[1].GetT(), 1.0));
}

TEST_CASE("A sphere is behind a ray")
{
    Ray R(Point(0.f, 0.f, 5.f), Vector(0.f, 0.f, 1.f));
    Sphere S;

    auto XS = Intersect(R, S);
    CHECK(XS.size() == 2);
    CHECK(Util::Equal(XS[0].GetT(), -6.0));
    CHECK(Util::Equal(XS[1].GetT(), -4.0));
}

TEST_CASE("Intersect a world with a ray")
{
    World W = World::DefaultWorld();
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));

    auto XS = Intersect(R, W);
    CHECK(Util::Equal(XS[0].GetT(), 4.f));
    CHECK(Util::Equal(XS[1].GetT(), 4.5f));
    CHECK(Util::Equal(XS[2].GetT(), 5.5f));
    CHECK(Util::Equal(XS[3].GetT(), 6.f));

    // the following value is 4 because two intersections keep a shared_ptr to the object
    // another owner is World
    // the last user is the pointer that prints use_count()
    // std::cout << "Object 1 num user: " << XS[1].GetObject().use_count() << '\n';
}

TEST_CASE("Precomputing the state of an intersection")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere Shape(1);
    auto I = Intersection<Sphere>(4.f, Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.AObject->GetID() == Shape.GetID());
    CHECK(Comps.Position == Point(0.f, 0.f, -1.f));
    CHECK(Comps.EyeV == Vector(0.f, 0.f, -1.f));
    CHECK(Comps.NormalV == Vector(0.f, 0.f, -1.f));
}

TEST_CASE("The hit, when an intersection occurs on the outside")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere Shape(1);
    auto I = Intersection<Object>(4.f, Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.IsInside == false);
}

TEST_CASE("The hit, when an intersection occurs on the inside")
{
    Ray R(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    Sphere Shape(1);
    auto I = Intersection<Sphere>(1.f, Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.Position == Point(0.f, 0.f, 1.f));
    CHECK(Comps.EyeV == Vector(0.f, 0.f, -1.f));
    CHECK(Comps.NormalV == Vector(0.f, 0.f, -1.f));
    CHECK(Comps.IsInside == true);
}

TEST_CASE("Shading an intersection")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    auto Shape = W.GetObjectAt(0);
    auto I = Intersection<Object>(4.f, Shape);
    auto Comps = I.PrepareComputations(R);
    auto C = ShadeHit(W, Comps);

    CHECK(C == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("Shading an intersection from the inside")
{
    auto W = World::DefaultWorld();
    W.SetLight(Light(Color(1.f, 1.f, 1.f), Point(0.f, 0.25f, 0.f)));

    Ray R(Point(0.f, 0.f, 0.f), Vector(0.f, 0.f, 1.f));
    auto Shape = W.GetObjectAt(1);
    auto I = Intersection<Object>(0.5f, Shape);
    auto Comps = I.PrepareComputations(R);
    
    bool IsInShadow = W.IsShadowed(Comps.OverPosition);
    std::cout << "IsInShadow: " << IsInShadow << '\n';
    std::cout << "Position: " << Comps.Position << '\n';
    std::cout << "OverPosition: " << Comps.OverPosition << '\n';

    auto C = ShadeHit(W, Comps);

    CHECK(C == Color(0.90498f, 0.90498f, 0.90498f));
    CHECK(Comps.IsInside == true);
}

TEST_CASE("Smart pointer and polymorphism")
{
    std::vector<std::unique_ptr<Object>> X;
    Sphere S;
    S.SetTransform(Matrix::Scaling(0.5f, 0.5f, 0.5f));
    auto V = S.NormalAt(Point(0.5f, 0.f, 0.f));

    // need to use make_unique, otherwise it will complain when free the memory
    X.push_back(std::make_unique<Sphere>(S));

    // std::cout << "Pointer location: " << &S << '\n';
    // std::cout << "Pointer location: " << X[0] << '\n';
    // std::cout << "Vector location: " << &X << '\n';
    // std::cout << "Normal vector value: " << V << '\n';
    // std::cout << "Normal vector value from vector X: " << X[0]->NormalAt(Point(0.5f, 0.f, 0.f)) << '\n';
}

TEST_CASE("The color when a ray misses")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 1.f, 0.f));

    auto C = ColorAt(W, R);
    CHECK(C == Color(0.f, 0.f, 0.f));
}

TEST_CASE("The color when a ray hits")
{
    auto W = World::DefaultWorld();
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));

    auto C = ColorAt(W, R);
    CHECK(C == Color(0.38066f, 0.47583f, 0.2855f));
}

TEST_CASE("The color with an intersection behind the ray")
{
    auto W = World::DefaultWorld();
    auto Outer = W.GetObjectAt(0);
    auto Mat = Outer->GetMaterial();
    Mat.SetAmbient(1.f);
    Outer->SetMaterial(Mat);

    auto Inner = W.GetObjectAt(1);
    Mat = Inner->GetMaterial();
    Mat.SetAmbient(1.f);
    Inner->SetMaterial(Mat);

    // std::cout << "Inner ambient:" << Inner->GetMaterial().GetAmbient() << '\n';

    Ray R(Point(0.f, 0.f, 0.75f), Vector(0.f, 0.f, -1.f));

    auto C = ColorAt(W, R);
    CHECK(C == Inner->GetMaterial().GetColor());
}

TEST_CASE("The hit should offset the point")
{
    Ray R(Point(0.f, 0.f, -5.f), Vector(0.f, 0.f, 1.f));
    Sphere Shape;
    Shape.SetTransform(Matrix::Translation(0.f, 0.f, 1.f));
    Intersection I(5.f, Shape);
    auto Comps = I.PrepareComputations(R);

    CHECK(Comps.OverPosition.Z() < -Util::EPSILON/2);
    CHECK(Comps.Position.Z() > Comps.OverPosition.Z());
}