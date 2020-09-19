// #include "include/Util.h"
#include <cmath>
// #include <iostream>
// #include <exception>
#include <memory>
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

Color Lighting(Material &M, Light &L, Point &Pos, Vector &EyeV, Vector &NormalV, bool IsInShadow)
{
    // use color from pattern if applicable
    Color ColorUsed;
    if (M.GetPattern())
        ColorUsed = M.GetPattern()->StripeAt(Pos);
    else
        ColorUsed = M.GetColor();

    // combine the surface color with the light's color/intensity
    auto EffectiveColor = ColorUsed * L.GetIntensity();

    // find the direction of the light source
    auto LightV = (L.GetPosition() - Pos).Normalize();

    auto Ambient = EffectiveColor * M.GetAmbient();
    // note that a color is black by default
    Color Diffuse, Specular;

    if (IsInShadow)
        return Ambient;

    // LightDotNormal represents the cosine of the angle between the light vector
    // and the normal vector.
    // A negative value means the light is on the other side of the surface
    auto LightDotNormal = LightV.Dot(NormalV);

    if (LightDotNormal >= 0.)
    {
        Diffuse = EffectiveColor * M.GetDiffuse() * LightDotNormal;

        // ReflectDotEye represents the cosine of the angle between the
        // reflection vector and the eye vector.
        // A negative value means the light reflects away from the eye.
        auto ReflectV = (-LightV).Reflect(NormalV);
        double ReflectDotEye = ReflectV.Dot(EyeV);

        if (ReflectDotEye > 0.)
        {
            // compute the specular contribution
            auto Factor = std::pow(ReflectDotEye, M.GetShininess());
            Specular = L.GetIntensity() * M.GetSpecular() * Factor;
        }
    }

    return Ambient + Diffuse + Specular;
}

Color Lighting(Material &&M, Light &L, Point &Pos, Vector &EyeV, Vector &NormalV, bool IsInShadow)
{
    return Lighting(M, L, Pos, EyeV, NormalV, IsInShadow);
}

TEST_CASE("A point light has a position and intensity")
{
    Color I(1., 1., 1.);
    Point P(0., 0., 0.);
    Light L(I, P);
    
    CHECK(L.GetIntensity() == I);
    CHECK(L.GetPosition() == P);
}

TEST_CASE("Lighting with the eye between the light and the surface")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., 0., -1.);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 0., -10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, false);
    CHECK(Result == Color(1.9, 1.9, 1.9));
}

TEST_CASE("Lighting with the eye between the light and the surface, eye offset 45 degrees")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., std::sqrt(2.) / 2, -std::sqrt(2.) / 2);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 0., -10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, false);
    CHECK(Result == Color(1.0, 1.0, 1.0));
}

TEST_CASE("Lighting with the eye between the light and the surface, light offset 45 degrees")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., 0., -1.);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 10., -10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, false);
    CHECK(Result == Color(0.7364, 0.7364, 0.7364));
}

TEST_CASE("Lighting with the eye in the path of the reflection vector")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., -std::sqrt(2.) / 2, -std::sqrt(2.) / 2);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 10., -10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, false);
    CHECK(Result == Color(1.63639, 1.63639, 1.63639));
}

TEST_CASE("Lighting with the light behind the surface")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., 0., -1.);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 0., 10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, false);
    CHECK(Result == Color(0.1, 0.1, 0.1));
}

TEST_CASE("Lighting with the surface in shadow")
{
    Material M;
    Point Pos(0., 0., 0.);
    Vector EyeV(0., 0., -1.);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 0., -10.));
    auto Result = Lighting(M, L, Pos, EyeV, NormalV, true);
    CHECK(Result == Color(0.1, 0.1, 0.1));
}

TEST_CASE("Lighting with a pattern applied")
{
    Material M;
    std::shared_ptr<Pattern> SP = std::make_shared<StripePattern>(Pattern::White, Pattern::Black);
    M.SetPattern(SP);
    M.SetAmbient(1.);
    M.SetDiffuse(0.);
    M.SetSpecular(0.);

    Point Pos1(0.9, 0., 0.);
    Point Pos2(1.1, 0., 0.);

    Vector EyeV(0., 0., -1.);
    Vector NormalV(0., 0., -1.);
    Light L(Color(1., 1., 1.), Point(0., 0., -10.));

    auto C1 = Lighting(M, L, Pos1, EyeV, NormalV, true);
    auto C2 = Lighting(M, L, Pos2, EyeV, NormalV, true);

    CHECK(C1 == Color(1., 1., 1.));
    CHECK(C2 == Color(0., 0., 0.));
}