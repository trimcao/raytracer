#include "include/Functions.h"
#include "include/Sphere.h"
#include "include/Transformations.h"
#include "include/Plane.h"
#include <cmath>

Color TRay::PatternAtShape(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P)
{
    auto LocalPos = Obj->GetTransform().Inverse().Mul(P);
    auto PatternPos = Pat->GetTransform().Inverse().Mul(LocalPos);
    return Pat->PatternAt(PatternPos);
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