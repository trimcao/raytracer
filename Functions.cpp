#include "include/Functions.h"
#include "include/Object.h"
#include "include/Sphere.h"
#include "include/Transformations.h"
#include "include/Plane.h"
#include "include/Intersection.h"
#include <vector>
#include <cmath>

Color TRay::PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P)
{
    auto LocalPos = Obj->GetTransform().Inverse().Mul(P);
    auto PatternPos = Pat->GetTransform().Inverse().Mul(LocalPos);
    return Pat->PatternAt(PatternPos);
}

template<class OT>
std::vector<double> TRay::ComputeRefractiveIndex(Intersection<OT> &I, std::vector<Intersection<OT>> IntersectionList)
{
    std::vector<double> NS;

    std::vector<std::shared_ptr<Object>> Containers;
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
    Comps.NormalV = Comps.AObject->NormalAt(Comps.Position);
    Comps.IsInside = false;

    if (Comps.NormalV.Dot(Comps.EyeV) < 0.)
    {
        Comps.IsInside = true;
        Comps.NormalV = -Comps.NormalV;
    }

    Comps.OverPosition = Comps.Position + Comps.NormalV * Util::EPSILON;

    Comps.ReflectV = R.GetDirection().Reflect(Comps.NormalV);

    // determine N1 and N2
    auto NS = ComputeRefractiveIndex(I, IntersectionList);
    Comps.N1 = NS[0];
    Comps.N2 = NS[1];

    return Comps;
}

TEST_CASE("Stripes with an object transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));
    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    auto C = TRay::PatternAtShape(Pat, Obj, Point(1.5, 0., 0.));

    CHECK(C == Pattern::White);
}

TEST_CASE("Stripes with a pattern transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    Pat->SetTransform(Transformations::Scaling(2., 2., 2.));

    auto C = TRay::PatternAtShape(Pat, Obj, Point(1.5, 0., 0.));

    CHECK(C == Pattern::White);
}

TEST_CASE("Stripes with both an object and a pattern transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    Pat->SetTransform(Transformations::Translation(0.5, 0., 0.));

    auto C = TRay::PatternAtShape(Pat, Obj, Point(2.5, 0., 0.));

    CHECK(C == Pattern::White);
}

TEST_CASE("A pattern with an object transformation")
{
    std::shared_ptr<Object> S = std::make_shared<Sphere>();
    S->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();

    auto C = TRay::PatternAtShape(TP, S, Point(2., 3., 4.));

    CHECK(C == Color(1., 1.5, 2.));
}

TEST_CASE("A pattern with a pattern transformation")
{
    std::shared_ptr<Object> S = std::make_shared<Sphere>();

    std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();
    TP->SetTransform(Transformations::Scaling(2., 2., 2.));

    auto C = TRay::PatternAtShape(TP, S, Point(2., 3., 4.));

    CHECK(C == Color(1., 1.5, 2.));
}

TEST_CASE("A pattern with both an object and a pattern transformation")
{
    std::shared_ptr<Object> S = std::make_shared<Sphere>();
    S->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> TP = std::make_shared<TestPattern>();
    TP->SetTransform(Transformations::Translation(0.5, 1., 1.5));

    auto C = TRay::PatternAtShape(TP, S, Point(2.5, 3., 3.5));

    CHECK(C == Color(0.75, 0.5, 0.25));
}

TEST_CASE("Precomputing the reflection vector")
{
    Plane P;
    Ray R(Point(0., 1., -1.), Vector(0., -std::sqrt(2.)/2, std::sqrt(2.)/2));
    Intersection I(std::sqrt(2.), P);
    auto Comps = I.PrepareComputations(R);
    CHECK(Comps.ReflectV == Vector(0., std::sqrt(2.)/2, std::sqrt(2.)/2));
}

TEST_CASE("Finding refractive indices n1 and n2 at various intersections")
{
    auto A = Sphere::GlassSphere();
    auto Mat = A.GetMaterial();
    Mat.SetRefractiveIndex(1.5);
    A.SetMaterial(Mat);
    A.SetTransform(Transformations::Scaling(2., 2., 2.));

    auto B = Sphere::GlassSphere();
    Mat = B.GetMaterial();
    Mat.SetRefractiveIndex(2.);
    B.SetMaterial(Mat);
    B.SetTransform(Transformations::Translation(0., 0., -0.25));

    auto C = Sphere::GlassSphere();
    Mat = C.GetMaterial();
    Mat.SetRefractiveIndex(2.5);
    C.SetMaterial(Mat);
    C.SetTransform(Transformations::Translation(0., 0., 0.25));

    Ray R(Point(0., 0., -4.), Vector(0., 0., 1.));

    auto APtr = std::make_shared<Sphere>(A);
    auto BPtr = std::make_shared<Sphere>(B);
    auto CPtr = std::make_shared<Sphere>(C);

    auto I0 = Intersection<Object>(2., APtr);
    auto I1 = Intersection<Object>(2.75, BPtr);
    auto I2 = Intersection<Object>(3.25, CPtr);
    auto I3 = Intersection<Object>(4.75, BPtr);
    auto I4 = Intersection<Object>(5.25, CPtr);
    auto I5 = Intersection<Object>(6., APtr);

    std::vector<Intersection<Object>> XS {I0, I1, I2, I3, I4, I5};

    auto Comps = TRay::PrepareComputations(XS[0], R, XS);
    CHECK(Util::Equal(Comps.N1, 1.));
    CHECK(Util::Equal(Comps.N2, 1.5));

    Comps = TRay::PrepareComputations(XS[1], R, XS);
    CHECK(Util::Equal(Comps.N1, 1.5));
    CHECK(Util::Equal(Comps.N2, 2.));

    Comps = TRay::PrepareComputations(XS[2], R, XS);
    CHECK(Util::Equal(Comps.N1, 2.));
    CHECK(Util::Equal(Comps.N2, 2.5));

    Comps = TRay::PrepareComputations(XS[3], R, XS);
    CHECK(Util::Equal(Comps.N1, 2.5));
    CHECK(Util::Equal(Comps.N2, 2.5));

    Comps = TRay::PrepareComputations(XS[4], R, XS);
    CHECK(Util::Equal(Comps.N1, 2.5));
    CHECK(Util::Equal(Comps.N2, 1.5));

    Comps = TRay::PrepareComputations(XS[5], R, XS);
    CHECK(Util::Equal(Comps.N1, 1.5));
    CHECK(Util::Equal(Comps.N2, 1.));
}