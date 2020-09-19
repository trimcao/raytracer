#include "include/Pattern.h"
#include <cmath>
#include "include/Transformations.h"

const Color Pattern::Black = Color(0., 0., 0.);
const Color Pattern::White = Color(1., 1., 1.);

Pattern::Pattern()
{
}

TestPattern::TestPattern()
{
    Transform = Matrix::Identity();
}

Color TestPattern::PatternAt(Point &P)
{
    return Color(P.X(), P.Y(), P.Z());
}

StripePattern::StripePattern(const Color &C1, const Color &C2)
{
    A = C1;
    B = C2;
    Transform = Matrix::Identity();
}

Color StripePattern::PatternAt(Point &P)
{
    int FloorX = std::floor(P.X());
    if (FloorX % 2 == 0)
    {
        return A;
    }

    return B;
}

TEST_CASE("Creating a stripe pattern")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.GetA() == Pattern::White);
    CHECK(SP.GetB() == Pattern::Black);
}

TEST_CASE("A stripe pattern is constant in y")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(0., 1., 0.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(0., 2., 0.)) == Pattern::White);
}

TEST_CASE("A stripe pattern is constant in z")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(0., 0., 1.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(0., 0., 2.)) == Pattern::White);
}

TEST_CASE("A stripe pattern alternates in x")
{
    StripePattern SP(Pattern::White, Pattern::Black);

    CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(0.9, 0., 0.)) == Pattern::White);
    CHECK(SP.PatternAt(Point(1., 0., 0.)) == Pattern::Black);
    CHECK(SP.PatternAt(Point(-0.1, 0., 0.)) == Pattern::Black);
    CHECK(SP.PatternAt(Point(-1., 0., 0.)) == Pattern::Black);
    CHECK(SP.PatternAt(Point(-1.1, 0., 0.)) == Pattern::White);
}

TEST_CASE("The default pattern transformation")
{
    TestPattern P;
    CHECK(P.GetTransform() == Matrix::Identity());
}

TEST_CASE("Assigning a transformation")
{
    TestPattern P;
    P.SetTransform(Transformations::Translation(1., 2., 3.));
    CHECK(P.GetTransform() == Transformations::Translation(1., 2., 3.));
}
