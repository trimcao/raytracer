#include "include/Pattern.h"
#include <cmath>

const Color Pattern::Black = Color(0., 0., 0.);
const Color Pattern::White = Color(1., 1., 1.);

Pattern::Pattern()
{
}

StripePattern::StripePattern(const Color &C1, const Color &C2)
{
    A = C1;
    B = C2;
    Transform = Matrix::Identity();
}

Color StripePattern::StripeAt(Point &P)
{
    int FloorX = std::floor(P.X());
    if (FloorX % 2 == 0)
    {
        return A;
    }

    return B;
}

// Color StripePattern::StripeAtObject(std::shared_ptr<Object> &Obj, Point &P)
// {
//     auto LocalPos = Obj->GetTransform().Inverse().Mul(P);
//     auto TransformedPos = Transform.Inverse().Mul(LocalPos);
//     return StripeAt(TransformedPos);
// }

TEST_CASE("Creating a stripe pattern")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.GetA() == Pattern::White);
    CHECK(SP.GetB() == Pattern::Black);
}

TEST_CASE("A stripe pattern is constant in y")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.StripeAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(0., 1., 0.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(0., 2., 0.)) == Pattern::White);
}

TEST_CASE("A stripe pattern is constant in z")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.StripeAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(0., 0., 1.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(0., 0., 2.)) == Pattern::White);
}

TEST_CASE("A stripe pattern alternates in x")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.StripeAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(0.9, 0., 0.)) == Pattern::White);
    CHECK(SP.StripeAt(Point(1., 0., 0.)) == Pattern::Black);
    CHECK(SP.StripeAt(Point(-0.1, 0., 0.)) == Pattern::Black);
    CHECK(SP.StripeAt(Point(-1., 0., 0.)) == Pattern::Black);
    CHECK(SP.StripeAt(Point(-1.1, 0., 0.)) == Pattern::White);
}