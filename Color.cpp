// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "include/Color.h"

Color::Color() {
    R = 0.f;
    G = 0.f;
    B = 0.f;
}

Color::Color(float R, float G, float B)
{
    this->R = R;
    this->G = G;
    this->B = B;
}

std::vector<int> Color::ToPPMVal(int MaxColorValue)
{
    int Red = std::round(MaxColorValue * this->R);
    Red = std::clamp(Red, 0, MaxColorValue);
    int Green = std::round(MaxColorValue * this->G);
    Green = std::clamp(Green, 0, MaxColorValue);
    int Blue = std::round(MaxColorValue * this->B);
    Blue = std::clamp(Blue, 0, MaxColorValue);

    std::vector<int> result = {Red, Green, Blue};
    return result;
}

std::ostream &operator<<(std::ostream &os, const Color &C)
{
    os << "Color{ " << C.R << ", " << C.G << ", " << C.B << " }";
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

TEST_CASE("convert color to ppm string")
{
    Color A = Color(1.5f, 0.f, 0.f);
    Color B = Color(0.f, 0.5f, 0.f);
    Color C = Color(-0.5f, 0.f, 1.f);
    std::vector<int> APPM = {255, 0, 0};
    std::vector<int> BPPM = {0, 128, 0};
    std::vector<int> CPPM = {0, 0, 255};
    
    CHECK(A.ToPPMVal(255) == APPM);
    CHECK(B.ToPPMVal(255) == BPPM);
    CHECK(C.ToPPMVal(255) == CPPM);
}