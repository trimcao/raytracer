#include "include/Util.h"
#include "include/Material.h"

Material::Material()
{
    AColor = Color(1., 1., 1.);
    Ambient = 0.1;
    Diffuse = 0.9;
    Specular = 0.9;
    Shininess = 200.;
    APattern = nullptr;
    Reflective = 0.;
    Transparency = 0.;
    RefractiveIndex = 1.;
}

Material::Material(Color &C, double Amb, double Dif, double Spec, double Shini)
{
    AColor = C;
    Ambient = Amb;
    Diffuse = Dif;
    Specular = Spec;
    Shininess = Shini;
    APattern = nullptr;
    Reflective = 0.;
    Transparency = 0.;
    RefractiveIndex = 1.;
}

Material::Material(Color &&C, double Amb, double Dif, double Spec, double Shini) : Material(C, Amb, Dif, Spec, Shini)
{
}

bool operator==(const Material &LHS, const Material &RHS)
{
    return (LHS.GetColor() == RHS.GetColor() && Util::Equal(LHS.GetAmbient(), RHS.GetAmbient())
            && Util::Equal(LHS.GetDiffuse(), RHS.GetDiffuse())
            && Util::Equal(LHS.GetSpecular(), RHS.GetSpecular())
            && Util::Equal(LHS.GetShininess(), RHS.GetShininess()));
}

bool operator!=(const Material &LHS, const Material &RHS)
{
    return !(LHS == RHS);
}

// TEST_CASE("The default material")
// {
//     Material M;

//     CHECK(M.GetColor() == Color(1., 1., 1.));
//     CHECK(Util::Equal(M.GetAmbient(), 0.1));
//     CHECK(Util::Equal(M.GetDiffuse(), 0.9));
//     CHECK(Util::Equal(M.GetSpecular(), 0.9));
//     CHECK(Util::Equal(M.GetShininess(), 200.));
//     CHECK(Util::Equal(M.GetReflective(), 0.));
//     CHECK(Util::Equal(M.GetTransparency(), 0.));
//     CHECK(Util::Equal(M.GetRefractiveIndex(), 1.));
// }
