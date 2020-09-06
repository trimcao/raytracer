#pragma once

#include "doctest.h"
#include "Object.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Material.h"

class Sphere : public Object
{
    Matrix Transform;
    Point Origin;
    Material AMaterial;

public:
    Sphere();
    Sphere(int ID);

    int GetID();

    inline Matrix GetTransform() const { return Transform; }
    inline Material GetMaterial() const { return AMaterial; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

    Vector NormalAt(Point &P);
    Vector NormalAt(Point &&P);
};