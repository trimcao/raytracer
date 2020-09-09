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

public:
    Sphere(int ID);
    Sphere();

    int GetID();

    // inline Matrix GetTransform() const { return Transform; }
    // inline Material GetMaterial() const { return AMaterial; }

    // inline void SetTransform(Matrix &M) { Transform = M; }
    // inline void SetTransform(Matrix &&M) { Transform = M; }

    // inline void SetMaterial(Material &M) { AMaterial = M; }
    // inline void SetMaterial(Material &&M) { AMaterial = M; }

    virtual Vector NormalAt(Point &P) override;
    virtual Vector NormalAt(Point &&P) override;
};