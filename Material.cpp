#include "include/Util.h"
#include "include/Material.h"

Material::Material()
{
    AColor = Color(1.f, 1.f, 1.f);
    Ambient = 0.1f;
    Diffuse = 0.9f;
    Specular = 0.9f;
    Shininess = 200.f;
}

Material::Material(Color &C, float Amb, float Diff, float Spec, float Shini)
{
    AColor = C;
    Ambient = Amb;
    Diffuse = Diff;
    Specular = Spec;
    Shininess = Shini;
}

Material::Material(Color &&C, float Amb, float Diff, float Spec, float Shini) : Material(C, Amb, Diff, Spec, Shini)
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

TEST_CASE("The default material")
{
    Material M;

    CHECK(M.GetColor() == Color(1.f, 1.f, 1.f));
    CHECK(Util::Equal(M.GetAmbient(), 0.1f));
    CHECK(Util::Equal(M.GetDiffuse(), 0.9f));
    CHECK(Util::Equal(M.GetSpecular(), 0.9f));
    CHECK(Util::Equal(M.GetShininess(), 200.f));
}
