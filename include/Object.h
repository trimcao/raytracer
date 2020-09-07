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

    virtual ~Object()=default;

    Matrix GetTransform() const { return Transform; }
    Material GetMaterial() const { return AMaterial; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }

    inline void SetMaterial(Material &M) { AMaterial = M; }
    inline void SetMaterial(Material &&M) { AMaterial = M; }

    virtual Vector NormalAt(Point &P) = 0;
    virtual Vector NormalAt(Point &&P) = 0;
};