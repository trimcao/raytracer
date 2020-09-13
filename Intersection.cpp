#include <iostream>
#include <exception>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <cmath>
#include "include/Intersection.h"
#include "include/Ray.h"
// #include "include/Sphere.h"
#include "include/Util.h"
// #include "include/Light.h"

template <class OT>
Intersection<OT>::Intersection() {}

template <class OT>
double Intersection<OT>::GetT() const
{
    return T;
}

template<class OT>
std::shared_ptr<OT> Intersection<OT>::GetObject() const
{
    return O;
}

template<class OT>
Intersection<OT>::Intersection(double T, const OT &O)
{
    this->T = T;
    this->O = std::make_shared<OT>(O);
}

template<class OT>
Intersection<OT>::Intersection(double T, std::shared_ptr<OT> &O)
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
        if (I.GetT() > 0.)
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

    if (Comps.NormalV.Dot(Comps.EyeV) < 0.)
    {
        Comps.IsInside = true;
        Comps.NormalV = -Comps.NormalV;
    }

    Comps.OverPosition = Comps.Position + Comps.NormalV * Util::EPSILON;

    return Comps;
}

// std::vector<Intersection<Sphere>> Intersect(const Ray &R, const Sphere &S)
// {
//     std::vector<Intersection<Sphere>> Intersections;

//     auto TransformedRay = R.Transform(S.GetTransform().Inverse());

//     // assume the origin of Sphere is always (0., 0., 0.)
//     Vector SphereToRay = TransformedRay.GetOrigin() - Point(0., 0., 0.);
//     double A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
//     double B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
//     double C = SphereToRay.Dot(SphereToRay) - 1.;

//     double Discriminant = B * B - 4 * A * C;

//     if (Discriminant >= 0.)
//     {
//         Intersections.push_back(Intersection((-B - std::sqrt(Discriminant)) / (2 * A), S));
//         Intersections.push_back(Intersection((-B + std::sqrt(Discriminant)) / (2 * A), S));
//     }

//     return Intersections;
// }

// std::vector<Intersection<Object>> Intersect(const Ray &R, const World &W)
// {
//     std::vector<Intersection<Object>> Intersections;

//     // now all objects are sphere
//     for (auto O : W.GetObjects())
//     {
//         auto TransformedRay = R.Transform(O->GetTransform().Inverse());

//         // assume the origin of Sphere is always (0., 0., 0.)
//         Vector SphereToRay = TransformedRay.GetOrigin() - Point(0., 0., 0.);
//         double A = TransformedRay.GetDirection().Dot(TransformedRay.GetDirection());
//         double B = 2 * TransformedRay.GetDirection().Dot(SphereToRay);
//         double C = SphereToRay.Dot(SphereToRay) - 1.;

//         double Discriminant = B * B - 4 * A * C;

//         if (Discriminant >= 0.)
//         {
//             Intersections.push_back(Intersection((-B - std::sqrt(Discriminant)) / (2 * A), O));
//             Intersections.push_back(Intersection((-B + std::sqrt(Discriminant)) / (2 * A), O));
//         }
//     }

//     // sort the intersections
//     std::sort(Intersections.begin(), Intersections.end());

//     return Intersections;
// }

// Color ShadeHit(World &W, PreComputations<Object> &Comps, bool RenderShadow)
// {
//     if (!W.GetLight())
//         return Color(0., 0., 0.);

//     bool IsInShadow = false;
//     if (RenderShadow)
//         IsInShadow = W.IsShadowed(Comps.OverPosition);

//     return Lighting(Comps.AObject->GetMaterial(), *W.GetLight(), Comps.OverPosition, Comps.EyeV, Comps.NormalV, IsInShadow);
// }

// Color ColorAt(World &W, Ray &R, bool RenderShadow)
// {
//     auto Intersects = Intersect(R, W);
//     auto H = Hit(Intersects);
//     if (H != nullptr)
//     {
//         auto Comps = H->PrepareComputations(R);
//         return ShadeHit(W, Comps, RenderShadow);
//     }

//     return Color(0., 0., 0.);
// }


// TEST_CASE("An intersection encapsulates t and object")
// {
//     Sphere S(3);
//     Intersection I(3.5, S);

//     CHECK(Util::Equal(I.GetT(), 3.5));
//     CHECK(I.GetObject()->GetID() == 3);
// }

// TEST_CASE("The hit, when all intersections have positive t")
// {
//     Sphere S(1);
//     Intersection I1(1., S);
//     Intersection I2(2., S);
//     auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

//     CHECK(*Hit(XS) == I1);
// }

// TEST_CASE("The hit, when some intersections have negative t")
// {
//     Sphere S(1);
//     Intersection I1(-1., S);
//     Intersection I2(1., S);
//     auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

//     CHECK(*Hit(XS) == I2);
// }

// TEST_CASE("The hit, when all intersections have negative t")
// {
//     Sphere S(1);
//     Intersection I1(-2., S);
//     Intersection I2(-1., S);
//     auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2});

//     CHECK(Hit(XS) == nullptr);
// }

// TEST_CASE("The hit, when all intersections have negative t")
// {
//     Sphere S(1);
//     Intersection I1(5., S);
//     Intersection I2(7., S);
//     Intersection I3(-3., S);
//     Intersection I4(2., S);
//     auto XS = Intersections(std::vector<Intersection<Sphere>> {I1, I2, I3, I4});

//     CHECK(*Hit(XS) == I4);
// }


// TEST_CASE("The hit should offset the point")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     Sphere Shape;
//     Shape.SetTransform(Matrix::Translation(0., 0., 1.));
//     Intersection I(5., Shape);
//     auto Comps = I.PrepareComputations(R);

//     CHECK(Comps.OverPosition.Z() < -Util::EPSILON/2);
//     CHECK(Comps.Position.Z() > Comps.OverPosition.Z());
// }