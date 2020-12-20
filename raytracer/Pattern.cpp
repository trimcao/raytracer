#include "include/Pattern.h"
#include <cmath>
#include "include/Transformations.h"

const Color Pattern::Black = Color(0., 0., 0.);
const Color Pattern::White = Color(1., 1., 1.);

Pattern::Pattern()
{
}

void Pattern::SetTransform(Matrix &T)
{
    Transform = T;
    TransformInverse = Transform.Inverse();
}

TestPattern::TestPattern()
{
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
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
    TransformInverse = Matrix::Identity();
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

GradientPattern::GradientPattern(const Color &C1, const Color &C2)
{
    A = C1;
    B = C2;
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
}

Color GradientPattern::PatternAt(Point &P)
{
    auto Distance = B - A;
    auto Fraction = P.X() - std::floor(P.X());

    return A + Distance * Fraction;
}

RingPattern::RingPattern(const Color &C1, const Color &C2)
{
    A = C1;
    B = C2;
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
}

Color RingPattern::PatternAt(Point &P)
{
    int DistanceFromOrigin = std::floor( std::sqrt( std::pow(P.X(), 2.) + std::pow(P.Z(), 2.) ) );

    if (DistanceFromOrigin % 2 == 0)
        return A;
    else
        return B;
}

CheckersPattern::CheckersPattern(const Color &C1, const Color &C2)
{
    A = C1;
    B = C2;
    Transform = Matrix::Identity();
    TransformInverse = Matrix::Identity();
}

Color CheckersPattern::PatternAt(Point &P)
{
    int DistanceFromOrigin = std::floor(P.X()) + std::floor(P.Y()) + std::floor(P.Z());

    if (DistanceFromOrigin % 2 == 0)
        return A;
    else
        return B;
}

// TEST_CASE("Creating a stripe pattern")
// {
//     StripePattern SP(Pattern::White, Pattern::Black);

//     CHECK(SP.GetA() == Pattern::White);
//     CHECK(SP.GetB() == Pattern::Black);
// }

// TEST_CASE("A stripe pattern is constant in y")
// {
//     StripePattern SP(Pattern::White, Pattern::Black);

//     CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(0., 1., 0.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(0., 2., 0.)) == Pattern::White);
// }

// TEST_CASE("A stripe pattern is constant in z")
// {
//     StripePattern SP(Pattern::White, Pattern::Black);

//     CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(0., 0., 1.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(0., 0., 2.)) == Pattern::White);
// }

// TEST_CASE("A stripe pattern alternates in x")
// {
//     StripePattern SP(Pattern::White, Pattern::Black);

//     CHECK(SP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(0.9, 0., 0.)) == Pattern::White);
//     CHECK(SP.PatternAt(Point(1., 0., 0.)) == Pattern::Black);
//     CHECK(SP.PatternAt(Point(-0.1, 0., 0.)) == Pattern::Black);
//     CHECK(SP.PatternAt(Point(-1., 0., 0.)) == Pattern::Black);
//     CHECK(SP.PatternAt(Point(-1.1, 0., 0.)) == Pattern::White);
// }

// TEST_CASE("The default pattern transformation")
// {
//     TestPattern P;
//     CHECK(P.GetTransform() == Matrix::Identity());
// }

// TEST_CASE("Assigning a transformation")
// {
//     TestPattern P;
//     P.SetTransform(Transformations::Translation(1., 2., 3.));
//     CHECK(P.GetTransform() == Transformations::Translation(1., 2., 3.));
// }

// TEST_CASE("A gradient linearly interpolates between colors")
// {
//     GradientPattern GP(Pattern::White, Pattern::Black);

//     CHECK(GP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(GP.PatternAt(Point(0.25, 0., 0.)) == Color(0.75, 0.75, 0.75));
//     CHECK(GP.PatternAt(Point(0.5, 0., 0.)) == Color(0.5, 0.5, 0.5));
//     CHECK(GP.PatternAt(Point(0.75, 0., 0.)) == Color(0.25, 0.25, 0.25));
// }

// TEST_CASE("A ring should extend in both x and z")
// {
//     RingPattern RP(Pattern::White, Pattern::Black);

//     CHECK(RP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(RP.PatternAt(Point(1., 0., 0.)) == Pattern::Black);
//     CHECK(RP.PatternAt(Point(0., 0., 1.)) == Pattern::Black);
//     CHECK(RP.PatternAt(Point(0.708, 0., 0.708)) == Pattern::Black);
// }

// TEST_CASE("Checkers should repeat in x")
// {
//     CheckersPattern CP(Pattern::White, Pattern::Black);

//     CHECK(CP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(0.99, 0., 0.)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(1.01, 0., 0.)) == Pattern::Black);
// }

// TEST_CASE("Checkers should repeat in y")
// {
//     CheckersPattern CP(Pattern::White, Pattern::Black);

//     CHECK(CP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(0., 0.99, 0.)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(0., 1.01, 0.)) == Pattern::Black);
// }

// TEST_CASE("Checkers should repeat in z")
// {
//     CheckersPattern CP(Pattern::White, Pattern::Black);

//     CHECK(CP.PatternAt(Point(0., 0., 0.)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(0., 0., 0.99)) == Pattern::White);
//     CHECK(CP.PatternAt(Point(0., 0., 1.01)) == Pattern::Black);
// }
