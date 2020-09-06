// #include "include/Matrix.h"
// #include "include/Util.h"
// #include <cmath>
// #include <iostream>
// #include <exception>
// #include <stdexcept>
#include "include/Light.h"

Light::Light()
{
}

Light::Light(Color &C, Point &P)
{
    Intensity = C;
    Position = P;
}

Light::Light(Color &&C, Point &&P) : Light(C, P)
{
}

TEST_CASE("A point light has a position and intensity")
{
    Color I(1.f, 1.f, 1.f);
    Point P(0.f, 0.f, 0.f);
    Light L(I, P);
    
    CHECK(L.GetIntensity() == I);
    CHECK(L.GetPosition() == P);
}

