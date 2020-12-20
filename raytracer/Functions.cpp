#include "include/Functions.h"
#include "include/Object.h"
#include "include/Sphere.h"
#include "include/Transformations.h"
#include "include/Plane.h"
#include "include/Intersection.h"
#include "include/Triangles.h"
#include <vector>
#include <cmath>

template PreComputations<Object> TRay::PrepareComputations(Intersection<Object> &I, Ray &R, std::vector<Intersection<Object>> IntersectionList);

Color TRay::PatternAtShape(std::shared_ptr<Pattern> &Pat, Object *Obj, Point &P)
{
    auto LocalPos = WorldToObject(Obj, P);
    auto PatternPos = Pat->GetTransformInverse().Mul(LocalPos);
    return Pat->PatternAt(PatternPos);
}

template<class OT>
std::vector<double> TRay::ComputeRefractiveIndex(Intersection<OT> &I, std::vector<Intersection<OT>> IntersectionList)
{
    std::vector<double> NS;

    std::vector<Object*> Containers;
    for (auto &Intersect : IntersectionList)
    {
        if (Intersect == I)
        {
            if (Containers.size() == 0)
            {
                NS.push_back(1.);
            } else
            {
                NS.push_back(Containers[Containers.size()-1]->GetMaterial().GetRefractiveIndex());
            }
        }

        // search containers for Intersect.GetObject()
        int ObjectIdx = -1;
        for (int j = 0; j < Containers.size(); ++j)
        {
            if (Containers[j] == Intersect.GetObject())
            {
                ObjectIdx = j;
            }
        }

        if (ObjectIdx == -1)
        {
            Containers.push_back(Intersect.GetObject());
        } else
        {
            Containers.erase(Containers.begin() + ObjectIdx);
        }

        if (Intersect == I)
        {
            if (Containers.size() == 0)
            {
                NS.push_back(1.);
            } else
            {
                NS.push_back(Containers[Containers.size()-1]->GetMaterial().GetRefractiveIndex());
            }

            break;
        }
    }

    return NS;
}

template<class OT>
PreComputations<OT> TRay::PrepareComputations(Intersection<OT> &I, Ray &R, std::vector<Intersection<OT>> IntersectionList)
{
    if (IntersectionList.size() == 0)
        IntersectionList.push_back(I);

    PreComputations<OT> Comps;
    Comps.T = I.GetT();
    Comps.AObject = I.GetObject();
    Comps.Position = R.Position(Comps.T);
    Comps.EyeV = -R.GetDirection();
    Comps.NormalV = Comps.AObject->NormalAt(Comps.Position, I);
    Comps.IsInside = false;

    if (Comps.NormalV.Dot(Comps.EyeV) < 0.)
    {
        Comps.IsInside = true;
        Comps.NormalV = -Comps.NormalV;
    }

    Comps.OverPosition = Comps.Position + Comps.NormalV * Util::EPSILON;
    Comps.UnderPosition = Comps.Position - Comps.NormalV * Util::EPSILON;

    Comps.ReflectV = R.GetDirection().Reflect(Comps.NormalV);

    // determine N1 and N2
    auto NS = ComputeRefractiveIndex(I, IntersectionList);
    Comps.N1 = NS[0];
    Comps.N2 = NS[1];

    return Comps;
}

float TRay::Schlick(PreComputations<Object> &Comps)
{
    // cos(theta_i) is the same as the dot product of the two vectors
    auto CosI = Comps.EyeV.Dot(Comps.NormalV);
    // total internal reflection can only occur if n1 > n2
    if (Comps.N1 > Comps.N2)
    {
        auto N = Comps.N1 / Comps.N2;
        auto Sin2T = N * N * (1. - CosI * CosI);

        if (Sin2T > 1.) return 1.;

        // compute cosine of theta_t using trig identity
        auto CosT = std::sqrt(1. - Sin2T);
        // when n1 > n2, use cos(theta_t) instead
        CosI = CosT;
    }

    auto R0 = std::pow( (Comps.N1 - Comps.N2) / (Comps.N1 + Comps.N2), 2. );

    return R0 + (1 - R0) * std::pow( (1 - CosI), 5.);
}

Point TRay::WorldToObject(Object *O, Point &P)
{
    Point NewPoint = P;

    if (O->GetParent())
    {
        NewPoint = WorldToObject(O->GetParent(), P);
    }

    return O->GetTransformInverse().Mul(NewPoint);
}

Vector TRay::NormalToWorld(Object *O, Vector &N)
{
    auto Normal = O->GetTransformInverse().T().Mul(N);
    Normal.SetW(0);
    Normal = Normal.Normalize();

    if (O->GetParent())
    {
        Normal = NormalToWorld(O->GetParent(), Normal);
    }

    return Normal;
}

// TEST_CASE("Stripes with an object transformation")
// {
//     std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
//     Obj->SetTransform(Transformations::Scaling(2., 2., 2.));
//     std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
//     auto C = TRay::PatternAtShape(Pat, Obj.get(), Point(1.5, 0., 0.));

//     CHECK(C == Pattern::White);
// }

// TEST_CASE("Stripes with a pattern transformation")
// {
//     std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
//     Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

//     std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
//     Pat->SetTransform(Transformations::Scaling(2., 2., 2.));

//     auto C = TRay::PatternAtShape(Pat, Obj.get(), Point(1.5, 0., 0.));

//     CHECK(C == Pattern::White);
// }

// TEST_CASE("Stripes with both an object and a pattern transformation")
// {
//     std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
//     Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

//     std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
//     Pat->SetTransform(Transformations::Translation(0.5, 0., 0.));

//     auto C = TRay::PatternAtShape(Pat, Obj.get(), Point(2.5, 0., 0.));

//     CHECK(C == Pattern::White);
// }

// TEST_CASE("A pattern with an object transformation")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>();
//     S->SetTransform(Transformations::Scaling(2., 2., 2.));

//     std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();

//     auto C = TRay::PatternAtShape(TP, S.get(), Point(2., 3., 4.));

//     CHECK(C == Color(1., 1.5, 2.));
// }

// TEST_CASE("A pattern with a pattern transformation")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>();

//     std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();
//     TP->SetTransform(Transformations::Scaling(2., 2., 2.));

//     auto C = TRay::PatternAtShape(TP, S.get(), Point(2., 3., 4.));

//     CHECK(C == Color(1., 1.5, 2.));
// }

// TEST_CASE("A pattern with both an object and a pattern transformation")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>();
//     S->SetTransform(Transformations::Scaling(2., 2., 2.));

//     std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();
//     TP->SetTransform(Transformations::Translation(0.5, 1., 1.5));

//     auto C = TRay::PatternAtShape(TP, S.get(), Point(2.5, 3., 3.5));

//     CHECK(C == Color(0.75, 0.5, 0.25));
// }

// TEST_CASE("Precomputing the reflection vector")
// {
//     Plane P;
//     Ray R(Point(0., 1., -1.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));
//     Intersection I(std::sqrt(2.), P);
//     auto Comps = I.PrepareComputations(R);
//     CHECK(Comps.ReflectV == Vector(0., std::sqrt(2.)/2, std::sqrt(2.)/2));
// }

// TEST_CASE("Finding refractive indices n1 and n2 at various intersections")
// {
//     auto A = Sphere::GlassSphere();
//     auto Mat = A.GetMaterial();
//     Mat.SetRefractiveIndex(1.5);
//     A.SetMaterial(Mat);
//     A.SetTransform(Transformations::Scaling(2., 2., 2.));

//     auto B = Sphere::GlassSphere();
//     Mat = B.GetMaterial();
//     Mat.SetRefractiveIndex(2.);
//     B.SetMaterial(Mat);
//     B.SetTransform(Transformations::Translation(0., 0., -0.25));

//     auto C = Sphere::GlassSphere();
//     Mat = C.GetMaterial();
//     Mat.SetRefractiveIndex(2.5);
//     C.SetMaterial(Mat);
//     C.SetTransform(Transformations::Translation(0., 0., 0.25));

//     Ray R(Point(0., 0., -4.), Vector(0., 0., 1.));

//     auto APtr = std::make_shared<Sphere>(A);
//     auto BPtr = std::make_shared<Sphere>(B);
//     auto CPtr = std::make_shared<Sphere>(C);

//     auto I0 = Intersection<Object>(2., APtr.get());
//     auto I1 = Intersection<Object>(2.75, BPtr.get());
//     auto I2 = Intersection<Object>(3.25, CPtr.get());
//     auto I3 = Intersection<Object>(4.75, BPtr.get());
//     auto I4 = Intersection<Object>(5.25, CPtr.get());
//     auto I5 = Intersection<Object>(6., APtr.get());

//     std::vector<Intersection<Object>> XS {I0, I1, I2, I3, I4, I5};

//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     CHECK(Util::Equal(Comps.N1, 1.));
//     CHECK(Util::Equal(Comps.N2, 1.5));

//     Comps = TRay::PrepareComputations(XS[1], R, XS);
//     CHECK(Util::Equal(Comps.N1, 1.5));
//     CHECK(Util::Equal(Comps.N2, 2.));

//     Comps = TRay::PrepareComputations(XS[2], R, XS);
//     CHECK(Util::Equal(Comps.N1, 2.));
//     CHECK(Util::Equal(Comps.N2, 2.5));

//     Comps = TRay::PrepareComputations(XS[3], R, XS);
//     CHECK(Util::Equal(Comps.N1, 2.5));
//     CHECK(Util::Equal(Comps.N2, 2.5));

//     Comps = TRay::PrepareComputations(XS[4], R, XS);
//     CHECK(Util::Equal(Comps.N1, 2.5));
//     CHECK(Util::Equal(Comps.N2, 1.5));

//     Comps = TRay::PrepareComputations(XS[5], R, XS);
//     CHECK(Util::Equal(Comps.N1, 1.5));
//     CHECK(Util::Equal(Comps.N2, 1.));
// }

// TEST_CASE("The under point is offset below the surface")
// {
//     Ray R(Point(0., 0., -5.), Vector(0., 0., 1.));
//     std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere::GlassSphere());
//     S->SetTransform(Transformations::Translation(0., 0., 1.));

//     Intersection I(5., S.get());
//     std::vector<Intersection<Object>> XS {I};

//     auto Comps = TRay::PrepareComputations(I, R, XS);
//     CHECK(Comps.UnderPosition.Z() > Util::EPSILON/2);
//     CHECK(Comps.Position.Z() < Comps.UnderPosition.Z());
// }

// TEST_CASE("The Schlick approximation under total internal reflection")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere::GlassSphere());
//     Ray R(Point(0., 0., std::sqrt(2.)/2), Vector(0., 1., 0.));
//     std::vector<Intersection<Object>> XS   {Intersection(-std::sqrt(2.)/2, S.get()),
//                                             Intersection(std::sqrt(2.)/2, S.get())};
//     auto Comps = TRay::PrepareComputations(XS[1], R, XS);
//     auto Reflectance = TRay::Schlick(Comps);

//     CHECK(Util::Equal(Reflectance, 1.));
// }

// TEST_CASE("The Schlick approximation with a perpendicular viewing angle")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere::GlassSphere());
//     Ray R(Point(0., 0., 0.), Vector(0., 1., 0.));
//     std::vector<Intersection<Object>> XS   {Intersection(-1., S.get()),
//                                             Intersection(1., S.get())};
//     auto Comps = TRay::PrepareComputations(XS[1], R, XS);
//     auto Reflectance = TRay::Schlick(Comps);

//     CHECK(Util::Equal(Reflectance, 0.04));
// }

// TEST_CASE("The Schlick approximation with a small angle and n2 > n1")
// {
//     std::shared_ptr<Object> S = std::make_shared<Sphere>(Sphere::GlassSphere());
//     Ray R(Point(0., 0.99, -2.), Vector(0., 0., 1.));
//     std::vector<Intersection<Object>> XS   {Intersection(1.8589, S.get())};
//     auto Comps = TRay::PrepareComputations(XS[0], R, XS);
//     auto Reflectance = TRay::Schlick(Comps);

//     CHECK(Util::Equal(Reflectance, 0.48873));
// }