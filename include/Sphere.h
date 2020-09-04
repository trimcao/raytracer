#pragma once

#include "doctest.h"
#include "Object.h"
#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include "Util.h"

class Sphere : public Object
{
    Matrix Transform;

public:
    Sphere();
    Sphere(int ID);

    int GetID();

    inline Matrix GetTransform() const { return Transform; }

    inline void SetTransform(Matrix &M) { Transform = M; }
    inline void SetTransform(Matrix &&M) { Transform = M; }
};