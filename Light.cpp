// #include "include/Util.h"
#include <cmath>
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

Color Lighting(Material &M, Light &L, Point &Pos, Vector &EyeV, Vector &NormalV)
{
    // combine the surface color with the light's color/intensity
    auto EffectiveColor = M.GetColor() * L.GetIntensity();

    // find the direction of the light source
    auto LightV = (L.GetPosition() - Pos).Normalize();

    auto Ambient = EffectiveColor * M.GetAmbient();
    // note that a color is black by default
    Color Diffuse, Specular;

    // LightDotNormal represents the cosine of the angle between the light vector
    // and the normal vector.
    // A negative value means the light is on the other side of the surface
    auto LightDotNormal = LightV.Dot(NormalV);

    if (LightDotNormal >= 0.f)
    {
        Diffuse = EffectiveColor * M.GetDiffuse() * LightDotNormal;

        // ReflectDotEye represents the cosine of the angle between the
        // reflection vector and the eye vector.
        // A negative value means the light reflects away from the eye.
        auto ReflectV = (-LightV).Reflect(NormalV);
        float ReflectDotEye = ReflectV.Dot(EyeV);

        if (ReflectDotEye > 0.f)
        {
            // compute the specular contribution
            auto Factor = std::pow(ReflectDotEye, M.GetShininess());
            Specular = L.GetIntensity() * M.GetSpecular() * Factor;
        }
    }

    return Ambient + Diffuse + Specular;
}

Color Lighting(Material &&M, Light &L, Point &Pos, Vector &EyeV, Vector &NormalV)
{
    return Lighting(M, L, Pos, EyeV, NormalV);
}

TEST_CASE("A point light has a position and intensity")
{
    Color I(1.f, 1.f, 1.f);
    Point P(0.f, 0.f, 0.f);
    Light L(I, P);
    
    CHECK(L.GetIntensity() == I);
    CHECK(L.GetPosition() == P);
}

TEST_CASE("Lighting with the eye between the light and the surface")
{
    Material M;
    Point Pos(0.f, 0.f, 0.f);
    Vector EyeV(0.f, 0.f, -1.f);
    Vector NormalV(0.f, 0.f, -1.f);
    Light L(Color(1.f, 1.f, 1.f), Point(0.f, 0.f, -10.f));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV);
    CHECK(Result == Color(1.9f, 1.9f, 1.9f));
}

TEST_CASE("Lighting with the eye between the light and the surface, eye offset 45 degrees")
{
    Material M;
    Point Pos(0.f, 0.f, 0.f);
    Vector EyeV(0.f, std::sqrt(2.f) / 2, -std::sqrt(2.f) / 2);
    Vector NormalV(0.f, 0.f, -1.f);
    Light L(Color(1.f, 1.f, 1.f), Point(0.f, 0.f, -10.f));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV);
    CHECK(Result == Color(1.0, 1.0f, 1.0f));
}

TEST_CASE("Lighting with the eye between the light and the surface, light offset 45 degrees")
{
    Material M;
    Point Pos(0.f, 0.f, 0.f);
    Vector EyeV(0.f, 0.f, -1.f);
    Vector NormalV(0.f, 0.f, -1.f);
    Light L(Color(1.f, 1.f, 1.f), Point(0.f, 10.f, -10.f));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV);
    CHECK(Result == Color(0.7364f, 0.7364f, 0.7364f));
}

TEST_CASE("Lighting with the eye in the path of the reflection vector")
{
    Material M;
    Point Pos(0.f, 0.f, 0.f);
    Vector EyeV(0.f, -std::sqrt(2.f) / 2, -std::sqrt(2.f) / 2);
    Vector NormalV(0.f, 0.f, -1.f);
    Light L(Color(1.f, 1.f, 1.f), Point(0.f, 10.f, -10.f));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV);
    CHECK(Result == Color(1.63639f, 1.63639f, 1.63639f));
}

TEST_CASE("Lighting with the light behind the surface")
{
    Material M;
    Point Pos(0.f, 0.f, 0.f);
    Vector EyeV(0.f, 0.f, -1.f);
    Vector NormalV(0.f, 0.f, -1.f);
    Light L(Color(1.f, 1.f, 1.f), Point(0.f, 0.f, 10.f));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV);
    CHECK(Result == Color(0.1f, 0.1f, 0.1f));
}
