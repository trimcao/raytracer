#pragma once

#include "doctest.h"
#include "Color.h"
#include "Pattern.h"
#include <memory>

class Material
{
    Color AColor;
    double Ambient;
    double Diffuse;
    double Specular;
    double Shininess;
    std::shared_ptr<Pattern> APattern;

public:
    Material();
    Material(Color &C, double Amb, double Dif, double Spec, double Shini);
    Material(Color &&C, double Amb, double Dif, double Spec, double Shini);

    inline double GetAmbient() const { return Ambient; }
    inline double GetDiffuse() const { return Diffuse; }
    inline double GetSpecular() const { return Specular; }
    inline double GetShininess() const { return Shininess; }
    inline Color GetColor() const { return AColor; }
    inline std::shared_ptr<Pattern> GetPattern() const { return APattern; }

    inline void SetAmbient(double Amb) { Ambient = Amb; }
    inline void SetDiffuse(double Diff) { Diffuse = Diff; }
    inline void SetSpecular(double Spec) { Specular = Spec; }
    inline void SetShininess(double Shini) { Shininess = Shini; }
    inline void SetColor(Color &C) { AColor = C; }
    inline void SetColor(Color &&C) { AColor = C; }

    inline void SetPattern(std::shared_ptr<Pattern> &P) { APattern = P; }
};

bool operator==(const Material &LHS, const Material &RHS);
bool operator!=(const Material &LHS, const Material &RHS);