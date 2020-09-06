#pragma once

#include "doctest.h"
// #include <iostream>
// #include "Util.h"
#include "Color.h"
// #include "Point.h"

class Material
{
    Color AColor;
    float Ambient;
    float Diffuse;
    float Specular;
    float Shininess;

public:
    Material();
    Material(Color &C, float Amb, float Diff, float Spec, float Shini);
    Material(Color &&C, float Amb, float Diff, float Spec, float Shini);

    inline float GetAmbient() const { return Ambient; }
    inline float GetDiffuse() const { return Diffuse; }
    inline float GetSpecular() const { return Specular; }
    inline float GetShininess() const { return Shininess; }
    inline Color GetColor() const { return AColor; }

    inline void SetAmbient(float Amb) { Ambient = Amb; }
    inline void SetDiffuse(float Diff) { Diffuse = Diff; }
    inline void SetSpecular(float Spec) { Specular = Spec; }
    inline void SetShininess(float Shini) { Shininess = Shini; }
    inline void SetColor(Color &C) { AColor = C; }
    inline void SetColor(Color &&C) { AColor = C; }
};

bool operator==(const Material &LHS, const Material &RHS);
bool operator!=(const Material &LHS, const Material &RHS);