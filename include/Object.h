#pragma once

#include "doctest.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"
#include "Material.h"

class Object
{

protected:
    int ID;
    Matrix Transform;
    Point Origin;
    Material AMaterial;

public:
    Object();

    int GetID();

    Matrix GetTransform() const { return Transform; }
    Material GetMaterial() const { return AMaterial; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

    // inline virtual Vector NormalAt(Point &P) { return P - Origin; }
    inline virtual Vector NormalAt(Point &P) { return Vector(0.f, 0.f, 0.f); }
    inline virtual Vector NormalAt(Point &&P) { return NormalAt(P); }
};