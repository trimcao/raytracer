// #define DOCTEST_CONFIG_DISABLE
#include "include/Color.h"

Color::Color() {}

Color::Color(float XIn, float YIn, float ZIn)
{
    X = XIn;
    Y = YIn;
    Z = ZIn;
    W = 0.f;
}

std::ostream &operator<<(std::ostream &os, const Color &P)
{
    os << "Color{ " << P.X << ", " << P.Y << ", " << P.Z << " }";
    return os;
}

TEST_CASE("adding colors")
{
    Color A = Color(0.9f, 0.6f, 0.75f);
    Color B = Color(0.7f, 0.1f, 0.25f);
    CHECK((A + B) == Color(1.6f, 0.7f, 1.0f));
}

TEST_CASE("subtracting colors")
{
    Color A = Color(0.9f, 0.6f, 0.75f);
    Color B = Color(0.7f, 0.1f, 0.25f);
    CHECK((A - B) == Color(0.2f, 0.5f, 0.5f));
}

TEST_CASE("multiplying a color by a scalar")
{
    Color A = Color(0.2f, 0.3f, 0.4f);
    CHECK((A * 2) == Color(0.4f, 0.6f, 0.8f));
    CHECK((2 * A) == Color(0.4f, 0.6f, 0.8f));
}

TEST_CASE("multiplying color")
{
    Color A = Color(1.f, 0.2f, 0.4f);
    Color B = Color(0.9f, 1.f, 0.1f);
    CHECK((A * B) == Color(0.9f, 0.2f, 0.04f));
}