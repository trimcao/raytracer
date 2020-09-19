#include "include/Functions.h"
#include "include/Sphere.h"
#include "include/Transformations.h"

Color Fns::StripeAtObject(std::shared_ptr<Pattern> &Pat, std::shared_ptr<Object> &Obj, Point &P)
{
    auto LocalPos = Obj->GetTransform().Inverse().Mul(P);
    auto PatternPos = Pat->GetTransform().Inverse().Mul(LocalPos);
    return Pat->StripeAt(PatternPos);
}

TEST_CASE("Stripes with an object transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));
    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    auto C = Fns::StripeAtObject(Pat, Obj, Point(1.5, 0., 0.));

    CHECK(C == Pattern::White);
}

TEST_CASE("Stripes with a pattern transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    Pat->SetTransform(Transformations::Scaling(2., 2., 2.));

    auto C = Fns::StripeAtObject(Pat, Obj, Point(1.5, 0., 0.));

    CHECK(C == Pattern::White);
}

TEST_CASE("Stripes with both an object and a pattern transformation")
{
    std::shared_ptr<Object> Obj = std::make_shared<Sphere>();
    Obj->SetTransform(Transformations::Scaling(2., 2., 2.));

    std::shared_ptr<Pattern> Pat = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    Pat->SetTransform(Transformations::Translation(0.5, 0., 0.));

    auto C = Fns::StripeAtObject(Pat, Obj, Point(2.5, 0., 0.));

    CHECK(C == Pattern::White);
}