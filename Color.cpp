// #define DOCTEST_CONFIG_DISABLE
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "include/Color.h"

Color::Color() {
    R = 0.;
    G = 0.;
    B = 0.;
}

Color::Color(double R, double G, double B)
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
    Color A = Color(0.9, 0.6, 0.75f);
    Color B = Color(0.7, 0.1, 0.25f);
    CHECK((A + B) == Color(1.6, 0.7, 1.0f));
}

TEST_CASE("subtracting colors")
{
    Color A = Color(0.9, 0.6, 0.75f);
    Color B = Color(0.7, 0.1, 0.25f);
    CHECK((A - B) == Color(0.2, 0.5, 0.5f));
}

TEST_CASE("multiplying a color by a scalar")
{
    Color A = Color(0.2, 0.3, 0.4f);
    CHECK((A * 2) == Color(0.4, 0.6, 0.8f));
    CHECK((2 * A) == Color(0.4, 0.6, 0.8f));
}

TEST_CASE("multiplying color")
{
    Color A = Color(1., 0.2, 0.4f);
    Color B = Color(0.9, 1., 0.1f);
    CHECK((A * B) == Color(0.9, 0.2, 0.04f));
}

TEST_CASE("convert color to ppm string")
{
    Color A = Color(1.5, 0., 0.f);
    Color B = Color(0., 0.5, 0.f);
    Color C = Color(-0.5, 0., 1.f);
    std::vector<int> APPM = {255, 0, 0};
    std::vector<int> BPPM = {0, 128, 0};
    std::vector<int> CPPM = {0, 0, 255};
    
    CHECK(A.ToPPMVal(255) == APPM);
    CHECK(B.ToPPMVal(255) == BPPM);
    CHECK(C.ToPPMVal(255) == CPPM);
}